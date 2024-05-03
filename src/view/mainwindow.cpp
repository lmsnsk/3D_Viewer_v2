#include "mainwindow.h"

#include <QColorDialog>
#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QProcess>
#include <QThread>
#include <sstream>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->moveForward, SIGNAL(clicked()), this, SLOT(move_foo()));
  connect(ui->moveBack, SIGNAL(clicked()), this, SLOT(move_foo()));
  connect(ui->rotateForward, SIGNAL(clicked()), this, SLOT(rotate_foo()));
  connect(ui->rotateBack, SIGNAL(clicked()), this, SLOT(rotate_foo()));
  connect(ui->scaleButton, SIGNAL(clicked()), this, SLOT(scale_foo()));
  connect(ui->saveSettings, SIGNAL(clicked()), this, SLOT(saveSettings()));
  directionButtonGroup = new QButtonGroup(this);
  directionButtonGroup->addButton(ui->direct_X, DIRECT_X);
  directionButtonGroup->addButton(ui->direct_Y, DIRECT_Y);
  directionButtonGroup->addButton(ui->direct_Z, DIRECT_Z);

  connect(directionButtonGroup, SIGNAL(buttonClicked(int)), this,
          SLOT(directionChanged(int)));

  mView.setParent(
      ui->viewer);  // Устанавливаем главный виджет в качестве родителя
  mView.setGeometry(0, 0, ui->viewer->width(),
                    ui->viewer->height());  // Устанавливаем геометрию glView

  mView.lineColor = QColor::fromRgb(255, 0, 0, 255);
  mView.vertexColor = QColor::fromRgb(255, 0, 0, 255);
  mView.plotColor = QColor::fromRgb(0, 0, 0, 255);
  mView.line_type = 0;
  mView.line_width = 1;
  mView.vertex_type = 0;
  mView.vertex_size = 5;
  mView.projection_type = 0;

  setSettings();
  ui->linesWidth->setValidator(new QIntValidator(1, 10, this));
  ui->vertexSize->setValidator(new QIntValidator(1, 20, this));
  ui->step_moving->setValidator(
      new QRegExpValidator(QRegExp("[0-9]*\\.?[0-9]*")));
  ui->angle_rotate->setValidator(
      new QRegExpValidator(QRegExp("[0-9]*\\.?[0-9]*")));
}

MainWindow::~MainWindow() {
  free_data(&myData);
  delete ui;
}

void MainWindow::on_openFile_clicked() {
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open File"), "", tr("Text Files (*.obj);;All Files (*.*)"));
  const char *filePath_C = nullptr;
  if (!filePath.isEmpty()) {
    QByteArray byteArray = filePath.toLocal8Bit();
    filePath_C = byteArray.data();
    // qDebug() << "Selected file path:" << filePath_C;
  }
  int error = parcer(filePath_C, &myData);

  if (error) {
    ui->ErrorField->setStyleSheet("color:red;background:black");
    ui->ErrorField->setText(" Ошибка чтения файла!");
    qDebug() << "Error reading file";
  } else {
    ui->ErrorField->setStyleSheet("color:#82ff7d;background:black");
    ui->ErrorField->setText(" Файл открыт успешно");
    auto_scaling(&myData);
    is_model_opened = true;
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    ui->labelName->setText("Название файла: " + fileName);
    ui->labelVer->setText("Количество вершин: " +
                          QString::number(myData.vertices_count));
    ui->labelPol->setText("Количество полигонов: " +
                          QString::number(myData.faces_count));
    ui->labelPath->setText(filePath);

    mView.drawScene(myData);
    mView.update();
  }
  // qDebug() << "id:" << id;
}

void MainWindow::move_foo() {
  if (!is_model_opened) return;
  QPushButton *move_button = (QPushButton *)sender();
  double sign = 1;
  if (move_button == ui->moveBack) sign = -1;

  double step = sign * ui->step_moving->text().toDouble();
  switch (direct) {
    case DIRECT_X:
      moving(&myData, step, 0, 0);
      break;
    case DIRECT_Y:
      moving(&myData, 0, step, 0);
      break;
    case DIRECT_Z:
      moving(&myData, 0, 0, step);
      break;
  }

  mView.drawScene(myData);
  mView.update();
}

void MainWindow::rotate_foo() {
  if (!is_model_opened) return;
  QPushButton *rotate_button = (QPushButton *)sender();
  double sign = 1;
  if (rotate_button == ui->rotateBack) sign = -1;

  double step = sign * ui->angle_rotate->text().toDouble();
  switch (direct) {
    case DIRECT_X:
      rotation_x(&myData, step);
      break;
    case DIRECT_Y:
      rotation_y(&myData, step);
      break;
    case DIRECT_Z:
      rotation_z(&myData, step);
      break;
  }

  mView.drawScene(myData);
  mView.update();
}

void MainWindow::directionChanged(int id) {
  switch (id) {
    case DIRECT_X:
      direct = DIRECT_X;
      break;
    case DIRECT_Y:
      direct = DIRECT_Y;
      break;
    case DIRECT_Z:
      direct = DIRECT_Z;
      break;
  }
  // qDebug() << "direct" << direct;
}

void MainWindow::scale_foo() {
  if (!is_model_opened) return;
  double scale = ui->scale_koef->text().toDouble();
  scaling(&myData, scale);

  mView.drawScene(myData);
  mView.update();
}

void MainWindow::on_linesColorBtn_clicked() {
  QColor color = QColorDialog::getColor(QColor(0, 0, 255, 255));
  if (color.isValid()) {
    mView.lineColor = color;
    if (!is_model_opened) return;
    mView.drawScene(myData);
    mView.update();
  }
}

void MainWindow::on_plotColorBtn_clicked() {
  QColor color = QColorDialog::getColor(QColor(0, 0, 0, 255));
  if (color.isValid()) {
    mView.plotColor = color;
    if (!is_model_opened) {
      mView.initializeGL();
      mView.update();
      return;
    }
    mView.drawScene(myData);
    mView.update();
  }
}

void MainWindow::on_linesType_activated(int index) {
  if (index == 1)
    mView.line_type = 1;
  else if (index == 0)
    mView.line_type = 0;
  if (!is_model_opened) return;
  mView.drawScene(myData);
  mView.update();
}

void MainWindow::on_linesWidthAccept_clicked() {
  if (!ui->linesWidth->text().isEmpty()) {
    mView.line_width = ui->linesWidth->text().toInt();
    if (!is_model_opened) return;
    mView.drawScene(myData);
    mView.update();
  }
}

void MainWindow::on_vertexType_activated(int index) {
  if (index == 0)
    mView.vertex_type = 0;
  else if (index == 1)
    mView.vertex_type = 1;
  else
    mView.vertex_type = 2;
  if (!is_model_opened) return;
  mView.drawScene(myData);
  mView.update();
}

void MainWindow::on_vertexColorBtn_clicked() {
  QColor color = QColorDialog::getColor(QColor(0, 0, 0, 255));
  if (color.isValid()) {
    mView.vertexColor = color;
    if (!is_model_opened) return;
    mView.drawScene(myData);
    mView.update();
  }
}

void MainWindow::on_vertexSizeAccept_clicked() {
  if (!ui->vertexSize->text().isEmpty()) {
    mView.vertex_size = ui->vertexSize->text().toInt();
    if (!is_model_opened) return;
    mView.drawScene(myData);
    mView.update();
  }
}

void MainWindow::on_perspective_stateChanged(int arg1) {
  mView.projection_type = arg1 ? 1 : 0;
  mView.initializeGL();
  if (!is_model_opened) return;
  mView.drawScene(myData);
  mView.update();
}

void MainWindow::saveSettings() {
  QString filePath = "settings.user";
  std::ofstream outFile(filePath.toStdString());
  if (outFile.is_open()) {
    writeSettings(outFile);
    outFile.close();
    ui->ErrorField->setStyleSheet("color:#82ff7d;background:black");
    ui->ErrorField->setText(" Состояние сохранено");
    qDebug() << "Settings file created and state saved successfully.";
  } else
    qDebug() << "Error creating settings file.";
}

void MainWindow::setSettings() {
  QString filePath = "settings.user";
  std::ifstream inFile(filePath.toStdString());
  if (inFile.is_open()) {
    readSettings(inFile);
    inFile.close();
    ui->ErrorField->setStyleSheet("color:#82ff7d;background:black");
    ui->ErrorField->setText(" Состояние считано");
    qDebug() << "Settings file set successfully.";
  } else
    qDebug() << "Error reading settings file.";
}

void MainWindow::writeSettings(std::ofstream &outFile) {
  outFile << "X: " << (ui->direct_X->isChecked() ? "1" : "0") << "\n";
  outFile << "Y: " << (ui->direct_Y->isChecked() ? "1" : "0") << "\n";
  outFile << "Z: " << (ui->direct_Z->isChecked() ? "1" : "0") << "\n";
  outFile << "move: " << ui->step_moving->text().toStdString() << "\n";
  outFile << "rotate: " << ui->angle_rotate->text().toStdString() << "\n";
  outFile << "scale: " << ui->scale_koef->text().toStdString() << "\n";
  outFile << "perspective: " << (ui->perspective->isChecked() ? "1" : "0")
          << "\n";
  outFile << "linesColor: " << mView.lineColor.red() << " "
          << mView.lineColor.green() << " " << mView.lineColor.blue() << " "
          << mView.lineColor.alpha() << "\n";
  outFile << "vertexColor: " << mView.vertexColor.red() << " "
          << mView.vertexColor.green() << " " << mView.vertexColor.blue() << " "
          << mView.vertexColor.alpha() << "\n";
  outFile << "plotColor: " << mView.plotColor.red() << " "
          << mView.plotColor.green() << " " << mView.plotColor.blue() << " "
          << mView.plotColor.alpha() << "\n";
  outFile << "linesType: " << ui->linesType->currentIndex() << "\n";
  outFile << "linesWidth: " << ui->linesWidth->text().toStdString() << "\n";
  outFile << "vertexType: " << ui->vertexType->currentIndex() << "\n";
  outFile << "vertexSize: " << ui->vertexSize->text().toStdString() << "\n";
}

void MainWindow::readSettings(std::ifstream &inFile) {
  std::string line;
  while (std::getline(inFile, line)) {
    std::istringstream iss(line);
    std::string key;
    iss >> key;

    if (key == "X:") {
      int state;
      iss >> state;
      if (state == 1) {
        ui->direct_X->setChecked(true);
        directionChanged(DIRECT_X);
      }
    } else if (key == "Y:") {
      int state;
      iss >> state;
      if (state == 1) {
        ui->direct_Y->setChecked(true);
        directionChanged(DIRECT_Y);
      }
    } else if (key == "Z:") {
      int state;
      iss >> state;
      if (state == 1) {
        ui->direct_Z->setChecked(true);
        directionChanged(DIRECT_Z);
      }
    } else if (key == "move:") {
      std::string value;
      iss >> value;
      ui->step_moving->setText(QString::fromStdString(value));
    } else if (key == "rotate:") {
      std::string value;
      iss >> value;
      ui->angle_rotate->setText(QString::fromStdString(value));
    } else if (key == "scale:") {
      std::string value;
      iss >> value;
      ui->scale_koef->setText(QString::fromStdString(value));
    } else if (key == "perspective:") {
      int state;
      iss >> state;
      ui->perspective->setChecked(state == 1);
    } else if (key == "linesColor:") {
      int r, g, b, a;
      iss >> r >> g >> b >> a;
      mView.lineColor.setRgb(r, g, b, a);
    } else if (key == "vertexColor:") {
      int r, g, b, a;
      iss >> r >> g >> b >> a;
      mView.vertexColor.setRgb(r, g, b, a);
    } else if (key == "plotColor:") {
      int r, g, b, a;
      iss >> r >> g >> b >> a;
      mView.plotColor.setRgb(r, g, b, a);
    } else if (key == "linesType:") {
      int index;
      iss >> index;
      ui->linesType->setCurrentIndex(index);
      mView.line_type = index;
    } else if (key == "linesWidth:") {
      std::string value;
      iss >> value;
      ui->linesWidth->setText(QString::fromStdString(value));
      if (!value.empty()) mView.line_width = std::stoi(value);
    } else if (key == "vertexType:") {
      int index;
      iss >> index;
      ui->vertexType->setCurrentIndex(index);
      mView.vertex_type = index;
    } else if (key == "vertexSize:") {
      std::string value;
      iss >> value;
      ui->vertexSize->setText(QString::fromStdString(value));
      if (!value.empty()) mView.vertex_size = std::stoi(value);
    }
  }
}

void MainWindow::on_getScreenBMP_clicked() {
  std::string img_name = "Screenshot_";
  img_name.append(std::to_string(bmp_counter++));
  img_name.append(".bmp");
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Open Address book"), QString::fromStdString(img_name),
      tr("Address book (*.bmp)"));

  QImage pic = mView.grabFrameBuffer();
  pic.save(fileName, "BMP");
}

void MainWindow::on_getScreenJPG_clicked() {
  std::string img_name = "Screenshot_";
  img_name.append(std::to_string(jpg_counter++));
  img_name.append(".jpeg");
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Open Address book"), QString::fromStdString(img_name),
      tr("Address book (*.jpeg)"));

  QImage pic = mView.grabFrameBuffer();
  pic.save(QString::fromStdString(img_name), "JPEG");
}

void MainWindow::on_getScreenGIF_clicked() {
  std::string img_name = "Animation_";
  img_name.append(std::to_string(gif_counter++));
  img_name.append(".gif");
  // Set the desired output GIF file name
  // QString gifFileName = "Animation.gif";
  QString gifFileName = QFileDialog::getSaveFileName(
      this, tr("Open Address book"), QString::fromStdString(img_name),
      tr("Address book (*.gif)"));
  // Set the total duration of the GIF in milliseconds
  int gifDurationMs = 5000;
  // Set the number of frames to capture
  int numFrames = 30;

  captureFramesAndCreateGIF(gifFileName, gifDurationMs, numFrames);
}

void MainWindow::captureFramesAndCreateGIF(const QString &outputFileName,
                                           int durationMs, int numFrames) {
  QString tempDir = QCoreApplication::applicationDirPath() + "/frames";
  QDir().mkpath(tempDir);

  for (int i = 0; i < numFrames; ++i) {
    QString frameFilePath = tempDir + QString("/frame_%1.png").arg(i);
    captureFrame(frameFilePath);
    QCoreApplication::processEvents();  // Allow the application to process
                                        // events
    QThread::msleep(durationMs / numFrames);
  }

  createGIF(tempDir, outputFileName, durationMs);

  // Clean up temporary frames
  QDir(tempDir).removeRecursively();
}

void MainWindow::captureFrame(const QString &outputFilePath) {
  unsigned int width = mView.width();
  unsigned int height = mView.height();

  unsigned char *output = new unsigned char[width * height * 4];
  glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, output);

  QImage image(output, width, height, QImage::Format_RGB32);
  image = image.mirrored();  // Flip the image vertically if needed

  image.save(outputFilePath);

  delete[] output;
}

void MainWindow::createGIF(const QString &inputDir,
                           const QString &outputFileName, int durationMs) {
  QStringList arguments;
  arguments << "-framerate"
            << "10"  // Set the frame rate (adjust as needed)
            << "-i" << inputDir + "/frame_%d.png"
            << "-t"
            << QString::number(durationMs)  // Duration per frame in seconds
            << "-y" << outputFileName;

  QProcess process;
  process.start("ffmpeg", arguments);
  process.waitForFinished();

  qDebug() << "GIF file created: " << outputFileName;
}
