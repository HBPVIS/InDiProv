#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <boost/thread.hpp>
#include "zmq.hpp"
#include "indiprov.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    /*cout << prov->setused(51,31, 21, "usedTime", "label", "location", "role", "type") << endl;
    cout << prov->setwasGeneratedBy(51,21,31,"time","label","location","role","type") << endl;
    cout << prov->setwasDerivedFrom(51,21,22,11,13,14,"label","type")<<endl;
    cout << prov->checkPass("wf3" , "wf3")<<endl;
    cout << prov->createWF("wf1" , "wf1")<<endl;*/
    context = new zmq::context_t(1);
    for (int i = 5556; i < 5560; ++i) {
        boost::thread (&MainWindow::EReciever,this,context,i);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::EReciever(zmq::context_t* context , int portNum)
{
        /*for (int thread_nbr = 0; thread_nbr != 5; thread_nbr++) {
            pthread_t worker;
            pthread_create (&worker, NULL, worker_routine, (void *) &context);
        }
        */
        InDiProv* prov=new InDiProv();
        int cnt=0;
        string msgstr("Message recieved from " + std::to_string(portNum));
        zmq::socket_t socket(*context, ZMQ_REP);
        string address(("tcp://*:"+std::to_string(portNum)));
        cout << address << " started . . ."<<endl;
        socket.bind(address.c_str());
        while (true) {
            zmq::message_t request;
            socket.recv(&request);
            char *tmp = (char*)request.data();

            string msgstr = prov->queryParsing(tmp);
            //cout << msgstr << endl;
            zmq::message_t reply (msgstr.size()+1);
            memcpy ((void *)reply.data(), msgstr.c_str(), msgstr.size()+1);
            socket.send (reply);
        }
}
