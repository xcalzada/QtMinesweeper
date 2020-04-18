#include <QMouseEvent>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QMenu>
#include <QMenuBar>

#include <ctime>

#include "minesweeper.h"

minesweeper::minesweeper(QWidget *parent)
	:QMainWindow(parent) { 

	qApp->installEventFilter(this);

	generateToolBar();
	generateWidgets();
	generateWindowStructure();
	connect(timer, SIGNAL(timeout()), this, SLOT(counter()));
	connect(button, SIGNAL(released()), this, SLOT(handleButton()));
	makeBoard();
}

minesweeper::~minesweeper()
{
	for (int j = 0; j < this->sizeX; ++j) {
		for (int i = 0; i < this->sizeY; ++i) {

			delete this->tablero[j][i];
		}
	}

	
	delete grid;
	delete timer;
	delete lcd1;
	delete lcd2;
	delete button;
	delete window;
}

void minesweeper::generateWindowStructure()
{
	QVBoxLayout *vbox = new QVBoxLayout(this);

	QHBoxLayout *hbox1 = new QHBoxLayout();
	QHBoxLayout *hbox2 = new QHBoxLayout();

	generateWidgets();
	generateWindowSize();

	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);

	hbox1->addWidget(lcd1);
	hbox1->addWidget(button);

	hbox1->addWidget(lcd2);
	hbox2->addLayout(grid);

	window->setLayout(vbox);

	setCentralWidget(window);
}

void minesweeper::generateWindowSize()
{
	grid->setHorizontalSpacing(100);
	grid->setVerticalSpacing(100);
	grid->setContentsMargins(0, 0, 0, 0);
	grid->maximumSize().setHeight(200);
	grid->maximumSize().setWidth(200);
	grid->setSizeConstraint(QGridLayout::SetMaximumSize);
	grid->setHorizontalSpacing(0);
	grid->setVerticalSpacing(0);

	lcd1->setFixedSize(75, 25);
	lcd2->setFixedSize(75, 25);
	button->setFixedSize(25, 25);
	button->setIconSize(QSize(25, 25));

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void minesweeper::generateWidgets()
{
	window = new QWidget();
	lcd1 = new QLCDNumber();	// Mines counter
	lcd2 = new QLCDNumber();	//Timer of each game
	button = new QPushButton();	//Button to restard the game
	grid = new QGridLayout();
	timer = new QTimer(this);
}

void minesweeper::generateToolBar()
{

	QAction *easy = new QAction("Easy", this);
	QAction *intermediate = new QAction("Intermediate", this);
	QAction *expert = new QAction("Expert", this);

	QAction *about = new QAction("About", this);

	QMenu *file;
	file = menuBar()->addMenu("&About");
	file->addAction(about);
	file = menuBar()->addMenu("&Level");
	file->addAction(easy);
	file->addAction(intermediate);
	file->addAction(expert);

	connect(easy, &QAction::triggered, this, &minesweeper::easyGame);
	connect(intermediate, &QAction::triggered, this, &minesweeper::intermidiateGame);
	connect(expert, &QAction::triggered, this, &minesweeper::expertGame);
}

void minesweeper::generateMine() {

	srand((unsigned)time(0));
	int randX = 0;
	int randY = 0;
	for (int index = 0; index < this->mine; index++) {

		randX = (rand() % this->sizeX);
		randY = (rand() % this->sizeY);

		if (tablero[randX][randY]->getState() == 0) {

			tablero[randX][randY]->setState(-1);
			continue;
		}
		index--;
	}
}

void minesweeper::generateNumbers() {

	for (int i = 0; i < this->sizeX; ++i) {
		for (int j = 0; j < this->sizeY; ++j) {

			if (this->tablero[i][j]->getState() == 0) {
				int boom = detectorAroundCell(i, j);
				this->tablero[i][j]->setState(boom);
			}
		}
	}
}


void minesweeper::deleteBoard(){

	for (int j = 0; j < this->sizeX; ++j) {
		for (int i = 0; i < this->sizeY; ++i) {

			delete this->tablero[j][i];
		}
	}
	makeBoard(); //Generate the new game
}

void minesweeper::deleteBoard(int x,int y, int m)
{
	for (int j = 0; j < this->sizeX; ++j) {
		for (int i = 0; i < this->sizeY; ++i) {

			delete this->tablero[j][i];
		}
	}

	this->sizeX = x;
	this->sizeY = y;
	this->mine = m;

	makeBoard(); //Generate the new game
}

void minesweeper::makeBoard() {

	//Modify the staticvariable  in the object cell to restart the game 
	cell newGame;
	newGame.setEndGame(false);
	newGame.resetCountFlagBomb();

	//Put default image on the button
	button->setIcon(QIcon("uiImages/faceSmile.png"));

	//Restard timer
	lcd2->display(0);
	this->times = 0;
	timer->start(1000);

	//Restart counter mine
	disCountMine = mine;
	lcd1->display(disCountMine);

	//Create the new cell object for the board
	for (int j = 0; j < this->sizeX; ++j) {
		for (int i = 0; i < this->sizeY; ++i) {
			this->tablero[j][i] = new cell;
			this->grid->addWidget(this->tablero[j][i], j, i);

		}
	}

	//Add the neighbors cells in the cell
	for (int j = 0; j < this->sizeX; ++j) {
		for (int i = 0; i < this->sizeY; ++i) {

			if ((i - 1) >= 0)	this->tablero[j][i]->setUp(this->tablero[j][i - 1]); //up

			if ((i + 1) < this->sizeX)	this->tablero[j][i]->setDown(this->tablero[j][i + 1]); //down

			if ((j - 1) >= 0)	this->tablero[j][i]->setRight(this->tablero[j - 1][i]); //right

			if ((j + 1) < this->sizeY)	this->tablero[j][i]->setLeft(this->tablero[j + 1][i]); //left
		}
	}
	
	//Generated mines
	generateMine();

	//Genered the values of the cells without mines
	generateNumbers();	
}

int minesweeper::detectorAroundCell(int x, int y) {

	int boom = 0;

	for (int i = x - 1; (i <= x + 1); ++i) {
		for (int j = y - 1; j <= y + 1; ++j) {

			if (i >= 0
				&& j >= 0
				&& i < sizeX
				&& j < sizeY) {

				if (!(this->tablero[i][j]->getState() == 0)
					&& this->tablero[i][j]->getState() == -1) {

					boom++;
				}				
			}
		}
	}	
	return boom;
}


bool minesweeper::eventFilter(QObject *watched, QEvent *event) {
	
	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mev = static_cast<QMouseEvent*>(event);

		if (watched->inherits("cell")) {

			cell * const labelCell = qobject_cast<cell *>(watched);
			int code = labelCell->eventCell(mev);

			
			if (code == 1) {  //open cell
				if (labelCell->getState() == -1) {
					timer->stop();					
					button->setIcon(QIcon("uiImages/faceSad.png"));
				}
			}
			else if (code == 2) { //flag
				disCountMine--;
				lcd1->display(disCountMine);
				if (labelCell->countFlagBomb == this->mine 
					&& disCountMine == 0) {

					button->setIcon(QIcon("uiImages/faceWin.png"));
					labelCell->setEndGame(true);
					timer->stop();
				}
			}
			else if (code == 3) { //unflag
				disCountMine++;
				lcd1->display(disCountMine);
			}			 
		}
	}	
	return false;
}

void minesweeper::handleButton() {
	deleteBoard();
}

void minesweeper::counter() {
	times++;
	lcd2->display(times);
	setFixedSize(minimumSizeHint());
}


void minesweeper::easyGame() {

	deleteBoard(9, 9, 10);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	this->updatesEnabled();	
}

void minesweeper::intermidiateGame() {

	deleteBoard(16, 16, 40);
}

void minesweeper::expertGame() {

	deleteBoard(16, 30, 99);
}