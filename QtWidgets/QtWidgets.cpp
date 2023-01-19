#include "QtWidgets.h"
#include "Pic_data.h"
#include <QDebug>
#include "ui_QtWidgets.h"
#include<time.h>

QtWidgets::QtWidgets(QWidget * parent)
    : QMainWindow(parent)
{
    ui = new Ui::QtWidgetsClass();
    ui->setupUi(this);
    Initmenu();
    //connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(FlipImage()));
    /*Path = "D:\\Desktop\\R-C.png";
    InitImage();
    ShowImage(image);
    connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(FlipImage()));*/
    
}

QtWidgets::~QtWidgets()
{
    delete ui;
    delete p;
}

//void QtWidgets::on_pushButton1_clicked()
//
//{
//    //FlipImage();
//    qDebug("pushButtonConnect1");
//
//}
//
//void QtWidgets::on_pushButton2_clicked()
//
//{
//    FlipImage();
//    //qDebug("pushButtonConnect2");
//
//}

QString QtWidgets::SelectFile_Info()
{
//    //定义文件对话框类
//    QFileDialog* fileDialog = new QFileDialog(this);
//    //定义文件对话框标题
//    fileDialog->setWindowTitle(QStringLiteral("选中文件"));
//    //设置默认文件路径
//    fileDialog->setDirectory(".");
//    //设置文件过滤器
//    fileDialog->setNameFilter(tr("File(*.*)"));
//    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
//    fileDialog->setFileMode(QFileDialog::ExistingFiles);
//    //设置视图模式
//    fileDialog->setViewMode(QFileDialog::Detail);
//    //打印所有选择的文件的路径
//    QStringList fileNames;
//    if (fileDialog->exec()) {
//        fileNames = fileDialog->selectedFiles();
//    }
//        使用 QFileDialog 类中的 getOpenFileName() 函数来获取选择的文件名，这个函数会以模态方式运行一个选择文件对话框。
//        选择多个文件名存放在 QStringList 类型变量中。
//        除了使用这些静态函数，还可以建立对话框对象来操作。
//        QFileDialog 类还提供了 getSaveFileName() 函数来实现保存文件对话框和文件另存为对话框。
//        QFileDialog 类还提供 getExistingDirectory() 函数来获取一个已存在的文件夹路径。
    QString filename = QFileDialog::getOpenFileName(this, QStringLiteral("选择图片对话框！"),"D:", QStringLiteral("图片文件(*png)"));
    return filename;
}

QString QtWidgets::SelectFile_out()
{
    QString filename = QFileDialog::getOpenFileName(this, QStringLiteral("选择图片对话框！"), "D:", QStringLiteral("图片文件(*png)"));
    return filename;
}

void QtWidgets::Initmenu()
{
    //初始化下拉菜单
    actionIpSet = new QAction("翻转", this);
    actionAuthorize = new QAction("选择图片", this);
    actionSaveas = new QAction("另存为", this);
    ui->menuView->addAction(actionIpSet);
    ui->menuView->addAction(actionAuthorize);
    ui->menuView->addAction(actionSaveas);
    ui->menuView->addSeparator();
    ui->menuView->setWindowFlags(ui->menuView->windowFlags() | Qt::FramelessWindowHint);
    ui->menuView->setAttribute(Qt::WA_TranslucentBackground);
    ui->menuView->setStyleSheet(" QMenu {border-radius:5px;font-family:'Microsoft Yahei';font-size:14px;color:#000000;}"
        " QMenu::item {height:30px; width:100px;padding-left:20px;border: 1px solid none;}"
        "QMenu::item:selected {background-color:rgb(0,120,215);\
                        padding-left:20px;border: 1spx solid rgb(65,173,255);}");
    connect(ui->menuView, &QMenu::triggered, this, &QtWidgets::slot_action_triggered);
}

void QtWidgets::FlipImageth()
{
    clock_t start, end;
    start = clock();
    if (useThread)
    {
        std::thread t0(FlipImage, p, useThread, ncolor, 0);
        std::thread t1(FlipImage, p, useThread, ncolor, 1);
        std::thread t2(FlipImage, p, useThread, ncolor, 2);
        if (ncolor == 4) {
            std::thread t3(FlipImage, p, useThread, ncolor, 3);
            t3.join();
        }
        t0.join();
        t1.join();
        t2.join();
        SetPixelColor();
    }
    else
    {
        FlipImage(p,useThread,ncolor,0);
        SetPixelColor();
    }
    end = clock();
    cout << "运行时间" << (double)(end - start) / CLOCKS_PER_SEC << endl;
    ShowImage(image);
}

void QtWidgets::SetPixelColor() {
    for (int m = 0; m < p->m_width; m++) {
        for (int y = 0; y < p->m_height; y++)
        {

            image->setPixelColor(m, y, QColor(p->m_rgba[0][y * p->m_width + m], p->m_rgba[1][y * p->m_width + m], p->m_rgba[2][y * p->m_width + m]));
        }
    }
}
void QtWidgets::slot_action_triggered(QAction* action)
{
    if (action == actionIpSet)
    {
        useThread = true;
        if (p->m_alpha_flag == true) ncolor = 4;
        else ncolor = 3;
        FlipImageth();
    }
    else if (action == actionAuthorize)
    {
        Path = SelectFile_Info().toStdString();
        InitImage();
        ShowImage(image);
    }
    else if (action == actionSaveas)
    {
        GetImagesName();
        cout << imagename;
        WritePNGFile(("output/" + imagename).c_str());
    }
}
void QtWidgets::InitImage()
{
    p = new Pic_data(Path);
    image = new QImage(p->m_width, p->m_height, QImage::Format::Format_RGB32);
    for (int i = 0; i < p->m_width; i++) {
        for (int j = 0; j < p->m_height; j++)  
            {
                image->setPixelColor(i,j, QColor(p->m_rgba[0][j* p->m_width + i], p->m_rgba[1][j * p->m_width + i], p->m_rgba[2][ j * p->m_width + i]));
            }
    }
}

void QtWidgets::GetImagesName()
{
    intptr_t hFile = 0;
    struct _finddata_t fileinfo;
    string m;

    hFile = _findfirst(m.assign(Path).append("\\ * .png").c_str(), &fileinfo);
    imagename = fileinfo.name;
    if (hFile != -1) {
        do {
            
        } while (_findnext(hFile, &fileinfo) == 0);
    }
}

int QtWidgets::WritePNGFile(const char* file_name)
{
    int j, i, temp, pos;
    png_byte color_type;

    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep* row_pointers;
    FILE* fp = fopen(file_name, "wb");
    if (!fp)
    {
        cout << "[write_png_file] File " << file_name << " could not be opened for writing" << endl;
        return -1;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
    {
        cout << file_name << ": [write_png_file] png_create_write_struct failed" << endl;
        return -1;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        cout << file_name << ": [write_png_file] png_create_info_struct failed" << endl;
        return -1;
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        cout << file_name << ": [write_png_file] Error during init_io" << endl;
        return -1;
    }
    png_init_io(png_ptr, fp);


    if (setjmp(png_jmpbuf(png_ptr)))
    {
        cout << file_name << ": [write_png_file] Error during writing header" << endl;
        return -1;
    }
    if (p->m_alpha_flag == true) color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    else color_type = PNG_COLOR_TYPE_RGB;

    png_set_IHDR(png_ptr, info_ptr, p->m_width, p->m_height,
        p->m_bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);


    if (setjmp(png_jmpbuf(png_ptr)))
    {
        cout << file_name << ": [write_png_file] Error during writing bytes" << endl;
        return -1;
    }
    if (p->m_alpha_flag == true) temp = (4 * p->m_width);
    else temp = (3 * p->m_width);

    pos = 0;
    row_pointers = (png_bytep*)malloc(p->m_height * sizeof(png_bytep));
    for (i = 0; i < p->m_height; i++)
    {
        row_pointers[i] = (png_bytep)malloc(sizeof(unsigned char) * temp);
        if (p->m_alpha_flag == true) {
            for (j = 0; j < temp; j += 4)
            {
                row_pointers[i][j] = p->m_rgba[0][pos]; // red
                row_pointers[i][j + 1] = p->m_rgba[1][pos]; // green
                row_pointers[i][j + 2] = p->m_rgba[2][pos];   // blue
                row_pointers[i][j + 3] = p->m_rgba[3][pos]; // alpha
                ++pos;
            }
        }
        else
        {
            for (j = 0; j < temp; j += 3)
            {
                row_pointers[i][j] = p->m_rgba[0][pos]; // red
                row_pointers[i][j + 1] = p->m_rgba[1][pos]; // green
                row_pointers[i][j + 2] = p->m_rgba[2][pos];   // blue
                ++pos;
            }
        }

    }
    png_write_image(png_ptr, row_pointers);

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        cout << file_name << ": [write_png_file] Error during end of write" << endl;
        return -1;
    }
    png_write_end(png_ptr, NULL);

    for (j = 0; j < p->m_height; j++)
        free(row_pointers[j]);
    free(row_pointers);

    fclose(fp);
    return 0;

}

void FlipImage(Pic_data* p,bool useThread,int ncolor,int w)
{
    if (useThread) {
        //image = new QImage(p->m_width, p->m_height, QImage::Format::Format_RGB32);
        //for (int n = 0; n < ncolor; ++n) {
            for (int i = 0; i < p->m_height; ++i) {
                int k = 0;
                for (int j = i * p->m_width; j < floor((i * p->m_width) + p->m_width / 2); ++j) {
                    char temp = p->m_rgba[w][(i + 1) * p->m_width - 1 - k];
                    p->m_rgba[w][(i + 1) * p->m_width - 1 - k] = p->m_rgba[w][j];
                    p->m_rgba[w][j] = temp;
                    k++;
                }
            }
       // }
    }
    else {
        //image = new QImage(p->m_width, p->m_height, QImage::Format::Format_RGB32);
        for (int n = 0; n < ncolor; ++n) {
            for (int i = 0; i < p->m_height; ++i) {
                int k = 0;
                for (int j = i * p->m_width; j < floor((i * p->m_width) + p->m_width / 2); ++j) {
                    char temp = p->m_rgba[n][(i + 1) * p->m_width - 1 - k];
                    p->m_rgba[n][(i + 1) * p->m_width - 1 - k] = p->m_rgba[n][j];
                    p->m_rgba[n][j] = temp;
                    k++;
                }
            }
        }
    }
}


void QtWidgets::ShowImage(QImage* image) {
        ui->label->setPixmap(QPixmap::fromImage(*image).scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
}



