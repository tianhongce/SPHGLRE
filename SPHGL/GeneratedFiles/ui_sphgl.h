/********************************************************************************
** Form generated from reading UI file 'sphgl.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPHGL_H
#define UI_SPHGL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SPHGLClass
{
public:
    QAction *actionLock;
    QAction *actionAbout;
    QAction *actionBackground;
    QAction *actionChange2D;
    QAction *actionBamBreak;
    QAction *actionWaterPipe;
    QAction *actionBam;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionNumParticles;
    QAction *actionShowSurface;
    QAction *actionShowParticle;
    QAction *actionDisField;
    QAction *actionShowGrid;
    QAction *actionOutputFrame;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *baseLayout;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_3;
    QMenu *menu_2;
    QMenu *menu_4;
    QMenu *menu_5;
    QMenu *menuSurface;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *modelToolBar;
    QToolBar *controlToolBar;
    QToolBar *displayToolBar;
    QToolBar *ohterToolBar;

    void setupUi(QMainWindow *SPHGLClass)
    {
        if (SPHGLClass->objectName().isEmpty())
            SPHGLClass->setObjectName(QStringLiteral("SPHGLClass"));
        SPHGLClass->resize(900, 600);
        actionLock = new QAction(SPHGLClass);
        actionLock->setObjectName(QStringLiteral("actionLock"));
        actionLock->setCheckable(true);
        actionLock->setChecked(false);
        QIcon icon;
        icon.addFile(QStringLiteral(":/SPHGL/Resources/lock_view.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLock->setIcon(icon);
        actionAbout = new QAction(SPHGLClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionBackground = new QAction(SPHGLClass);
        actionBackground->setObjectName(QStringLiteral("actionBackground"));
        actionBackground->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SPHGL/Resources/action_background.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBackground->setIcon(icon1);
        actionChange2D = new QAction(SPHGLClass);
        actionChange2D->setObjectName(QStringLiteral("actionChange2D"));
        actionChange2D->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/SPHGL/Resources/action_2D.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionChange2D->setIcon(icon2);
        actionBamBreak = new QAction(SPHGLClass);
        actionBamBreak->setObjectName(QStringLiteral("actionBamBreak"));
        actionWaterPipe = new QAction(SPHGLClass);
        actionWaterPipe->setObjectName(QStringLiteral("actionWaterPipe"));
        actionWaterPipe->setCheckable(true);
        actionWaterPipe->setChecked(false);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/SPHGL/Resources/water_pipe.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWaterPipe->setIcon(icon3);
        actionBam = new QAction(SPHGLClass);
        actionBam->setObjectName(QStringLiteral("actionBam"));
        actionBam->setCheckable(true);
        actionBam->setChecked(true);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/SPHGL/Resources/bam.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBam->setIcon(icon4);
        actionStart = new QAction(SPHGLClass);
        actionStart->setObjectName(QStringLiteral("actionStart"));
        actionStart->setCheckable(true);
        actionStart->setChecked(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/SPHGL/Resources/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon5);
        actionStop = new QAction(SPHGLClass);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        actionStop->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/SPHGL/Resources/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon6);
        actionNumParticles = new QAction(SPHGLClass);
        actionNumParticles->setObjectName(QStringLiteral("actionNumParticles"));
        actionShowSurface = new QAction(SPHGLClass);
        actionShowSurface->setObjectName(QStringLiteral("actionShowSurface"));
        actionShowSurface->setCheckable(true);
        actionShowSurface->setChecked(false);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/SPHGL/Resources/show_surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowSurface->setIcon(icon7);
        actionShowParticle = new QAction(SPHGLClass);
        actionShowParticle->setObjectName(QStringLiteral("actionShowParticle"));
        actionShowParticle->setCheckable(true);
        actionShowParticle->setChecked(true);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/SPHGL/Resources/show_particle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowParticle->setIcon(icon8);
        actionDisField = new QAction(SPHGLClass);
        actionDisField->setObjectName(QStringLiteral("actionDisField"));
        actionDisField->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/SPHGL/Resources/show_dis_field.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisField->setIcon(icon9);
        actionShowGrid = new QAction(SPHGLClass);
        actionShowGrid->setObjectName(QStringLiteral("actionShowGrid"));
        actionShowGrid->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/SPHGL/Resources/show_grid.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowGrid->setIcon(icon10);
        actionOutputFrame = new QAction(SPHGLClass);
        actionOutputFrame->setObjectName(QStringLiteral("actionOutputFrame"));
        actionOutputFrame->setCheckable(true);
        actionOutputFrame->setChecked(false);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/SPHGL/Resources/output.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOutputFrame->setIcon(icon11);
        centralWidget = new QWidget(SPHGLClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        baseLayout = new QVBoxLayout();
        baseLayout->setSpacing(6);
        baseLayout->setObjectName(QStringLiteral("baseLayout"));

        gridLayout->addLayout(baseLayout, 0, 0, 1, 1);

        SPHGLClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SPHGLClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_3 = new QMenu(menu);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        menu_5 = new QMenu(menuBar);
        menu_5->setObjectName(QStringLiteral("menu_5"));
        menuSurface = new QMenu(menuBar);
        menuSurface->setObjectName(QStringLiteral("menuSurface"));
        SPHGLClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SPHGLClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SPHGLClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SPHGLClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SPHGLClass->setStatusBar(statusBar);
        modelToolBar = new QToolBar(SPHGLClass);
        modelToolBar->setObjectName(QStringLiteral("modelToolBar"));
        SPHGLClass->addToolBar(Qt::TopToolBarArea, modelToolBar);
        controlToolBar = new QToolBar(SPHGLClass);
        controlToolBar->setObjectName(QStringLiteral("controlToolBar"));
        SPHGLClass->addToolBar(Qt::TopToolBarArea, controlToolBar);
        displayToolBar = new QToolBar(SPHGLClass);
        displayToolBar->setObjectName(QStringLiteral("displayToolBar"));
        SPHGLClass->addToolBar(Qt::TopToolBarArea, displayToolBar);
        ohterToolBar = new QToolBar(SPHGLClass);
        ohterToolBar->setObjectName(QStringLiteral("ohterToolBar"));
        SPHGLClass->addToolBar(Qt::TopToolBarArea, ohterToolBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menuSurface->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(actionLock);
        menu->addAction(actionChange2D);
        menu->addAction(menu_3->menuAction());
        menu_3->addAction(actionWaterPipe);
        menu_3->addAction(actionBam);
        menu_2->addAction(actionAbout);
        menu_4->addAction(actionStart);
        menu_4->addAction(actionStop);
        menu_5->addAction(actionBackground);
        menu_5->addAction(actionNumParticles);
        menu_5->addAction(actionOutputFrame);
        menuSurface->addAction(actionShowParticle);
        menuSurface->addAction(actionDisField);
        menuSurface->addAction(actionShowSurface);
        menuSurface->addAction(actionShowGrid);
        mainToolBar->addAction(actionLock);
        mainToolBar->addAction(actionBackground);
        mainToolBar->addAction(actionChange2D);
        modelToolBar->addAction(actionWaterPipe);
        modelToolBar->addAction(actionBam);
        controlToolBar->addAction(actionStart);
        controlToolBar->addAction(actionStop);
        displayToolBar->addAction(actionShowParticle);
        displayToolBar->addSeparator();
        displayToolBar->addAction(actionDisField);
        displayToolBar->addAction(actionShowSurface);
        displayToolBar->addSeparator();
        displayToolBar->addAction(actionShowGrid);
        ohterToolBar->addAction(actionOutputFrame);

        retranslateUi(SPHGLClass);
        QObject::connect(actionLock, SIGNAL(triggered()), SPHGLClass, SLOT(onLockView()));
        QObject::connect(actionBackground, SIGNAL(triggered()), SPHGLClass, SLOT(onSetBackground()));
        QObject::connect(actionChange2D, SIGNAL(triggered()), SPHGLClass, SLOT(onChange2D()));
        QObject::connect(actionBam, SIGNAL(triggered()), SPHGLClass, SLOT(onBamBreak()));
        QObject::connect(actionWaterPipe, SIGNAL(triggered()), SPHGLClass, SLOT(onWaterPipe()));
        QObject::connect(actionStart, SIGNAL(triggered()), SPHGLClass, SLOT(onStart()));
        QObject::connect(actionStop, SIGNAL(triggered()), SPHGLClass, SLOT(onStop()));
        QObject::connect(actionNumParticles, SIGNAL(triggered()), SPHGLClass, SLOT(onSetNumParticles()));
        QObject::connect(actionShowSurface, SIGNAL(triggered()), SPHGLClass, SLOT(onShowSurface()));
        QObject::connect(actionShowParticle, SIGNAL(triggered()), SPHGLClass, SLOT(onShowParticle()));
        QObject::connect(actionDisField, SIGNAL(triggered()), SPHGLClass, SLOT(onShowDisField()));
        QObject::connect(actionShowGrid, SIGNAL(triggered()), SPHGLClass, SLOT(onShowHashGrid()));
        QObject::connect(actionOutputFrame, SIGNAL(triggered()), SPHGLClass, SLOT(onOutputFrame()));

        QMetaObject::connectSlotsByName(SPHGLClass);
    } // setupUi

    void retranslateUi(QMainWindow *SPHGLClass)
    {
        SPHGLClass->setWindowTitle(QApplication::translate("SPHGLClass", "SPHGL", Q_NULLPTR));
        actionLock->setText(QApplication::translate("SPHGLClass", "\351\224\201\345\256\232", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("SPHGLClass", "About", Q_NULLPTR));
        actionBackground->setText(QApplication::translate("SPHGLClass", "\350\203\214\346\231\257", Q_NULLPTR));
        actionChange2D->setText(QApplication::translate("SPHGLClass", "2D", Q_NULLPTR));
        actionBamBreak->setText(QApplication::translate("SPHGLClass", "\345\235\235\345\264\251", Q_NULLPTR));
        actionWaterPipe->setText(QApplication::translate("SPHGLClass", "\346\260\264\346\265\201", Q_NULLPTR));
        actionBam->setText(QApplication::translate("SPHGLClass", "BamBreak", Q_NULLPTR));
        actionStart->setText(QApplication::translate("SPHGLClass", "\345\274\200\345\247\213", Q_NULLPTR));
        actionStop->setText(QApplication::translate("SPHGLClass", "\345\201\234\346\255\242", Q_NULLPTR));
        actionNumParticles->setText(QApplication::translate("SPHGLClass", "\347\262\222\345\255\220\346\225\260", Q_NULLPTR));
        actionShowSurface->setText(QApplication::translate("SPHGLClass", "\346\230\276\347\244\272\346\266\262\351\235\242", Q_NULLPTR));
        actionShowParticle->setText(QApplication::translate("SPHGLClass", "\346\230\276\347\244\272\347\262\222\345\255\220", Q_NULLPTR));
        actionDisField->setText(QApplication::translate("SPHGLClass", "\346\230\276\347\244\272\350\267\235\347\246\273\345\234\272", Q_NULLPTR));
        actionShowGrid->setText(QApplication::translate("SPHGLClass", "\346\230\276\347\244\272\347\275\221\346\240\274", Q_NULLPTR));
        actionOutputFrame->setText(QApplication::translate("SPHGLClass", "\350\276\223\345\207\272\345\270\247", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionOutputFrame->setToolTip(QApplication::translate("SPHGLClass", "\344\273\245\346\226\207\344\273\266\347\232\204\345\275\242\345\274\217\350\276\223\345\207\272\346\257\217\345\270\247", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        menu->setTitle(QApplication::translate("SPHGLClass", "\345\274\200\345\247\213", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("SPHGLClass", "\345\234\272\346\231\257", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("SPHGLClass", "\345\205\263\344\272\216", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("SPHGLClass", "\346\216\247\345\210\266", Q_NULLPTR));
        menu_5->setTitle(QApplication::translate("SPHGLClass", "\350\256\276\347\275\256", Q_NULLPTR));
        menuSurface->setTitle(QApplication::translate("SPHGLClass", "\346\265\201\344\275\223", Q_NULLPTR));
        modelToolBar->setWindowTitle(QApplication::translate("SPHGLClass", "toolBar", Q_NULLPTR));
        controlToolBar->setWindowTitle(QApplication::translate("SPHGLClass", "toolBar", Q_NULLPTR));
        displayToolBar->setWindowTitle(QApplication::translate("SPHGLClass", "toolBar", Q_NULLPTR));
        ohterToolBar->setWindowTitle(QApplication::translate("SPHGLClass", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SPHGLClass: public Ui_SPHGLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPHGL_H
