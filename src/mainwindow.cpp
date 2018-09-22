#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "file.h"

typedef unsigned int UInt;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    time=new QTimer;
    ui->setupUi(this);
    speed=1000;
    inittxt="";
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(slotOpenFileDialog()));
    connect(ui->RESET,SIGNAL(clicked()),time,SLOT(stop()));
    connect(ui->actionOpen,SIGNAL(triggered()),time,SLOT(stop()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(slotSaveFileDialog()));
    connect(time,SIGNAL(timeout()),this,SLOT(on_Next_clicked()));
    ui->Memory_content->setColumnWidth(0,120);
    ui->Memory_content->setColumnWidth(1,120);
    ui->Memory_content->setShowGrid(false);
    ui->Memory_content->verticalHeader()->setVisible(false);
    ui->Memory_content->setEditTriggers(QTableWidget::NoEditTriggers);
    //ui->line_num->setText(ui->hbtext->blockNumber());
    C=new controller();
    QString tmp=ui->hbtext->toPlainText();
    string s=tmp.toStdString();
    ofstream fout("Input.ys");
    fout<<s<<endl;
    fout.close();
    C->Initial("Input.ys");
    memory_begin=0;memory_end=2048;
    updata();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Next_clicked()
{
    if (C->stat==SAOK) C->next();
    updata();
}

void MainWindow::on_CLEAR_clicked()
{
    QString tmp="";
    ui->Memory_Place_1->setText(tmp);
    ui->Memory_Place_2->setText(tmp);
    memory_begin=0;memory_end=1000;
    //ui->Memory_content->setText(M->PRINTMEMORY(memory_begin,memory_end));
}

void MainWindow::on_Memory_GO_clicked()
{
    QString tmp1=ui->Memory_Place_1->text();
    QString tmp2=ui->Memory_Place_2->text();
    bool tb;
    memory_begin=tmp1.toInt(&tb,10);
    memory_end=tmp2.toInt(&tb,10);
    //ui->Memory_content->setText(M->PRINTMEMORY(memory_begin,memory_end));
    //M->PRINTMEMORY(memory_begin,memory_end);
    updataMemory();
}

void MainWindow::on_RESET_clicked()
{
    delete C;
    C=new controller();
    QString tmp=ui->hbtext->toPlainText();
    string s=tmp.toStdString();
    ofstream fout("Input.ys");
    fout<<s;
    fout.close();
    C->Initial("Input.ys");
    memory_begin=0;memory_end=2048;
    updata();
}

void MainWindow::on_RUN_clicked()
{
    time->start(speed);
}

void MainWindow::slotOpenFileDialog()
{
    /*
    getOpenFileName函数说明
    函数原形： QStringList QFileDialog::getOpenFileNames(
    QWidget * parent = 0,
    const QString & caption = QString(),    //  打开文件对话框的标题
    const QString & dir = QString(),            //  查找目录
    const QString & filter = QString(),     //  设置需要过滤的文件格式
    QString * selectedFilter = 0,
    Options options = 0) [static]
    */
    //---获取文件名
    QString fileName = QFileDialog :: getOpenFileName(this, NULL, NULL, " *.ys");

    //---打开文件并读取文件内容
    QFile file(fileName);

    //--打开文件成功
    if (file.open(QIODevice ::ReadOnly | QIODevice ::Text))
    {
        QTextStream textStream(&file);
        inittxt="";
        while (!textStream.atEnd())
        {
            //---QtextEdit按行显示文件内容
            inittxt+=textStream.readLine()+"\n";
        }
    }
    else    //---打开文件失败
    {
        /*
        information函数参数说明：
        函数原型
        QMessageBox::information(
        QWidget * parent,
        const QString & title,                  //--- 标题
        const QString & text,                   //---显示内容
        StandardButtons buttons = Ok,   //---OK按钮
        StandardButton defaultButton = NoButton)
        [static]
        */
        QMessageBox ::information(NULL, NULL, "open file error");
    }
    ui->hbtext->setPlainText(inittxt);
    on_RESET_clicked();
}

QString MainWindow::PRINTFstat(){
    QString tmp;
    if (C->F->stall==0) tmp="QTextEdit{color:#66FF00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    else tmp="QTextEdit{color:#cc0000;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    return tmp;
}

QString MainWindow::PRINTDstat(){
    QString tmp;
    if (C->D->bubble==true) tmp="QTextEdit{color:#CCCC00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    else if (C->D->p_stat==SAOK) tmp="QTextEdit{color:#66FF00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    else tmp="QTextEdit{color:#cc0000;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    return tmp;
}

QString MainWindow::PRINTEstat(){
    QString tmp;
    if (C->E->bubble==true) tmp="QTextEdit{color:#CCCC00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    else if (C->E->p_stat==SAOK) tmp="QTextEdit{color:#66FF00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    else tmp="QTextEdit{color:#cc0000;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    return tmp;
}

QString MainWindow::PRINTMstat(){
    QString tmp;
    if (C->M->bubble==true) tmp="QTextEdit{color:#CCCC00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    else if (C->M->p_stat==SAOK) tmp="QTextEdit{color:#66FF00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    else tmp="QTextEdit{color:#cc0000;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    return tmp;
}

QString MainWindow::PRINTWstat(){
    QString tmp;
    if (C->W->stall==1) tmp="QTextEdit{color:#CCCC00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    if (C->W->p_stat==SAOK) tmp="QTextEdit{color:#66FF00;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    else tmp="QTextEdit{color:#cc0000;font-weight: 700;background-color: #fff; border-style:outset;border: 1px solid;border-radius: 6px;border-color: #ccc;border-top-right-radius: 0;border-bottom-right-radius: 0;}";
    return tmp;
}

QString MainWindow::PRINTCC(){
    string tmp="";
    tmp=tmp+" ZF="+to_string((int)C->E->ZF)+" SF="+to_string((int)C->E->SF)+"\n";
    tmp=tmp+" OF="+to_string((int)C->E->OF);
    return QString::fromStdString(tmp);
}

QString MainWindow::PRINTREG(){
    string tmp="";
    tmp=tmp+"eax="+to_string(C->Reg[0])+"\n";
    tmp=tmp+"ecx="+to_string(C->Reg[1])+"\n";
    tmp=tmp+"edx="+to_string(C->Reg[2])+"\n";
    tmp=tmp+"ebx="+to_string(C->Reg[3])+"\n";
    tmp=tmp+"esp="+to_string(C->Reg[4])+"\n";
    tmp=tmp+"ebp="+to_string(C->Reg[5])+"\n";
    tmp=tmp+"esi="+to_string(C->Reg[6])+"\n";
    tmp=tmp+"edi="+to_string(C->Reg[7])+"\n";
    return QString::fromStdString(tmp);
}

void MainWindow::updata(){
    QString tmp="";
    ui->halt_signal->setText(tmp);

    if (C->stat==SHLT){
        tmp="The program end.";
        ui->halt_signal->setText(tmp);
    }
    //tmp="Decode";

    ui->F_stat->setStyleSheet(PRINTFstat());
    ui->D_stat->setStyleSheet(PRINTDstat());
    ui->E_stat->setStyleSheet(PRINTEstat());
    ui->M_stat->setStyleSheet(PRINTMstat());
    ui->W_stat->setStyleSheet(PRINTWstat());
    //ui->D_stat->setText(tmp);
    ui->clock->setText("clock"+QString::number(C->clock,10));
    ui->TJM->setText(PRINTCC());
    ui->Register_text->setText(PRINTREG());
    //ui->Memory_content->setText(M->PRINTMEMORY(memory_begin,memory_end));
    updataMemory();
    ui->predPC->setText("predPC="+QString::number(C->F->p_predPC,10));

    ui->D_icode->setText("icode="+QString::number(C->D->p_icode,10));
    ui->D_ifun->setText("ifun="+QString::number(C->D->p_ifun,10));
    ui->D_rA->setText( "rA="+QString::fromStdString(reg2str(C->D->p_rA)) );
    ui->D_rB->setText("rB="+QString::fromStdString(reg2str(C->D->p_rB)));
    ui->D_valC->setText("valC="+QString::number(C->D->p_valC,10));
    ui->D_valP->setText("valP="+QString::number(C->D->p_valP,10));

    ui->E_icode->setText("icode="+QString::number(C->E->p_icode,10));
    ui->E_ifun->setText("ifun="+QString::number(C->E->p_ifun,10));
    ui->E_valC->setText("valC="+QString::number(C->E->p_valC,10));
    ui->E_valA->setText("valA="+QString::number(C->E->p_valA,10));
    ui->E_valB->setText("valB="+QString::number(C->E->p_valB,10));
    ui->E_dstE->setText("dstE="+QString::fromStdString(reg2str(C->E->p_dstE)));
    ui->E_dstM->setText("dstM="+QString::fromStdString(reg2str(C->E->p_dstM)));
    ui->E_srcA->setText("srcA="+QString::fromStdString(reg2str(C->E->p_srcA)));
    ui->E_srcB->setText("srcB="+QString::fromStdString(reg2str(C->E->p_srcB)));

    ui->M_icode->setText("icode="+QString::number(C->M->p_icode,10));
    ui->M_cnd->setText("Cnd="+QString::number(C->M->p_Cnd,10));
    ui->M_valE->setText("valE="+QString::number(C->M->p_valE,10));
    ui->M_valA->setText("valA="+QString::number(C->M->p_valA,10));
    ui->M_dstE->setText("dstE="+QString::fromStdString(reg2str(C->M->p_dstE)));
    ui->M_dstM->setText("dstM="+QString::fromStdString(reg2str(C->M->p_dstM)));

    ui->W_icode->setText("icode="+QString::number(C->W->p_icode,10));
    ui->W_valE->setText("valE="+QString::number(C->W->p_valE,10));
    ui->W_valM->setText("valM="+QString::number(C->W->p_valM,10));
    ui->W_dstE->setText("dstE="+QString::fromStdString(reg2str(C->W->p_dstE)));
    ui->W_dstM->setText("dstE="+QString::fromStdString(reg2str(C->W->p_dstE)));

}

void MainWindow::slotSaveFileDialog(){
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save"),
            "",
            tr("*.ys"));
    //--打开文件成功
    QFile file(fileName);
    qDebug()<<fileName<<endl;
    if (!fileName.isNull() && file.open(QIODevice::WriteOnly))
    {
        QTextStream textStream(&file);
        //---QtextEdit按行显示文件内容
        //inittxt+=textStream.readLine()+"\n";
        textStream<<ui->hbtext->toPlainText();
    }
    else
    {
        QMessageBox ::information(NULL, NULL, "save file error");
    }
}

void MainWindow::updataMemory(){

    int x=memory_begin,y=memory_end,p=0;
    bool tmp;
    ui->Memory_content->setRowCount(y-x+1);
    for (int i=x;i<=y;++i){
        ++p;
        ui->Memory_content->setItem(p,0,new QTableWidgetItem("0x"+QString::number(i,16)));
        ui->Memory_content->setItem(p,1,new QTableWidgetItem("0x"+QString::number(C->memory[i],16)));
    }

    ui->Memory_content->setColumnWidth(0,120);
    ui->Memory_content->setColumnWidth(1,120);
}

void MainWindow::on_SPEED_valueChanged(int value)
{
    speed=1000/(value+1);
    if (time->isActive()) time->start(speed);
}

void MainWindow::on_STOP_clicked()
{
    time->stop();
}
