#include "sphgl.h"

SPHGL::SPHGL(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
	init();

}

SPHGL::~SPHGL()
{

}

void SPHGL::init()
{
	viewWidget = new ModelWidget(&ui);
	ui.baseLayout->addWidget(viewWidget);

}

void SPHGL::onLockView()
{
	if (ui.actionLock->isChecked() == true)
	{
		viewWidget->isLock = true;
	}
	else
	{
		viewWidget->isLock = false;
	}
}

void SPHGL::onSetBackground()
{
	if (ui.actionBackground->isChecked() == true)
	{
		viewWidget->isBackground = true;
	}
	else
	{
		viewWidget->isBackground = false;
	}
}

void SPHGL::onChange2D()
{
	if (ui.actionChange2D->isChecked() == true)
	{
		viewWidget->change2D(true);
	}
	else
	{
		viewWidget->change2D(false);
	}

	//2D需要使用WATER_PIPE模式
	if (ui.actionWaterPipe->isChecked() == false)
	{
		ui.actionWaterPipe->setChecked(true);
		viewWidget->setModel(WATER_PIPE);
		ui.actionBam->setChecked(false);
	}
}

void SPHGL::onBamBreak()
{
	//BAM_BREAK不允许2D
	if (ui.actionChange2D->isChecked() == true)
	{
		ui.actionChange2D->setChecked(false);
		viewWidget->change2D(false);
	}

	if (ui.actionBam->isChecked() == true)
	{
		viewWidget->setModel(BAM_BREAK);

		ui.actionWaterPipe->setChecked(false);
	}
}
void SPHGL::onWaterPipe()
{
	if (ui.actionWaterPipe->isChecked() == true)
	{
		viewWidget->setModel(WATER_PIPE);

		ui.actionBam->setChecked(false);
	}
}

void SPHGL::onStart()
{
	ui.actionStart->setChecked(true);
	ui.actionStop->setChecked(false);
	viewWidget->start();
}
void SPHGL::onStop()
{
	ui.actionStop->setChecked(true);
	ui.actionStart->setChecked(false);
	viewWidget->stop();
}

void SPHGL::onSetNumParticles()
{
	int numPar = QInputDialog::getInt(this, T_CHAR("粒子数"), T_CHAR("输入粒子总数"), 1000, 50, 1600000, 5);

	if (numPar > 0)
		viewWidget->setNumParticle(numPar);
}

void SPHGL::onShowParticle()
{
	if (ui.actionShowParticle->isChecked() == true)
	{
		viewWidget->showParticle(true);
	}
	else
	{
		viewWidget->showParticle(false);
	}
}

void SPHGL::onShowSurface()
{
	if (ui.actionShowSurface->isChecked() == true)
	{
		viewWidget->showSurface(true);
	}
	else
	{
		viewWidget->showSurface(false);
	}
}

void SPHGL::onShowDisField()
{
	if (ui.actionDisField->isChecked() == true)
	{
		viewWidget->showDisField(true);
	}
	else
	{
		viewWidget->showDisField(false);
	}
}

void SPHGL::onShowHashGrid()
{
	if (ui.actionShowGrid->isChecked() == true)
	{
		viewWidget->showHashGrid(true);
	}
	else
	{
		viewWidget->showHashGrid(false);
	}
}

void SPHGL::onOutputFrame()
{
	if (ui.actionOutputFrame->isChecked() == true)
	{
		viewWidget->setOutputFrame(true);
	}
	else
	{
		viewWidget->setOutputFrame(false);
	}
}