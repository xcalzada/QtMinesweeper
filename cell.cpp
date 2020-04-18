#include "cell.h"
#include <QMouseEvent>

bool cell::endGame = false;
int cell::countFlagBomb = 0;

cell::cell(QWidget *parent)
	:QLabel(parent) {
	this->setFixedSize(25, 25);
	this->setPixmap(QPixmap("uiImages/facingDown.png"));
	this->setScaledContents(true);
	this->state = 0;	
}
 
cell::~cell() {

}

const int cell::getState() const {
	
	return this->state;
}

int cell::isOpened() {

	return opened;
}

void cell::setState(int state) {
	this->state = state;
}

void cell::setImage(bool caso) {

	this->setFixedSize(25, 25);
	this->setScaledContents(true);

	if (caso = false) {
		this->setPixmap(QPixmap("uiImages/facingDown.png"));
	}

	switch (state)
	{
		case -1:
			setEndGame(true);
			this->setPixmap(QPixmap("uiImages/bomb.png"));
		break;

		case 0:
			this->setPixmap(QPixmap("uiImages/0.png"));
		break;

		case 1:
			this->setPixmap(QPixmap("uiImages/1.png"));
		break;

		case 2:
			this->setPixmap(QPixmap("uiImages/2.png"));
		break;

		case 3:
			this->setPixmap(QPixmap("uiImages/3.png"));
		break;

		case 4:
			this->setPixmap(QPixmap("uiImages/4.png"));
		break;

		case 5:
			this->setPixmap(QPixmap("uiImages/5.png"));
		break;

		case 6:
			this->setPixmap(QPixmap("uiImages/6.png"));
		break;

		case 7:
			this->setPixmap(QPixmap("uiImages/7.png"));
		break;

		case 8:
			this->setPixmap(QPixmap("uiImages/8.png"));
		break;
	}	
}


void cell::setEndGame(bool stateGame) {

	this->endGame = stateGame;
}

void cell::resetCountFlagBomb() {

	this->countFlagBomb = 0;
}

void cell::setUp(cell* celda) {
	if (celda != nullptr) {
		this->up = celda;
	}
}

void cell::setDown(cell* celda) {
	if (celda != nullptr) {
		this->down = celda;
	}
}

void cell::setRight(cell* celda) {
	if (celda != nullptr) {
		this->right = celda;
	}
}

void cell::setLeft(cell* celda) {
	if (celda != nullptr) {
		this->left = celda;
	}
}

void cell::cellOp() {

	this->opened = true;
	this->checkCells();
	this->setImage(false);
}

void cell::checkCells() {

	if (this->getState() == 0) {
		//check up
		if (this->up != nullptr
			&& !this->up->opened
			&& this->up->getState() >= 0) {

			this->up->cellOp();
		}
		//check down
		if (this->down != nullptr
			&& !this->down->opened
			&& this->down->getState() >= 0) {

			this->down->cellOp();
		}
		//check right
		if (this->right != nullptr
			&& !this->right->opened
			&& this->right->getState() >= 0) {

			this->right->cellOp();
		}
		//check left
		if (this->left != nullptr
			&& !this->left->opened
			&& this->left->getState() >= 0) {

			this->left->cellOp();
		}
	}
}

int cell::eventCell(QMouseEvent *ev) {

	if (!endGame) {
		if (ev->buttons() == Qt::RightButton 
			&& this->opened == false)
		{
			if (rightClick == 0)
			{
				this->setPixmap(QPixmap("uiImages/flagged.png"));
				rightClick++;
				if (this->getState() == -1)countFlagBomb++;
				return 2;
			}
			else if (rightClick == 1) {
				this->setPixmap(QPixmap("uiImages/incognita.png"));
				rightClick++;
				if (this->getState() == -1)countFlagBomb--;
				return 3;
			}
			else {
				this->setPixmap(QPixmap("uiImages/facingDown.png"));
				rightClick = 0;
			}
		}

		if (ev->buttons() == Qt::LeftButton 
			&& rightClick == 0)
		{
			this->opened = true;
			setImage(false);
			checkCells();
			return 1;
		}

		if (ev->buttons() == Qt::MiddleButton)
		{
			this->setPixmap(QPixmap("uiImages/incognita.png"));
			rightClick = 2;
		}
	}
	return 0;
}