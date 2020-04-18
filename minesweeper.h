#pragma once
#include "cell.h"

#include <QtCore/QCoreApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QLCDNumber>

class minesweeper : public QMainWindow {
	Q_OBJECT
public:
	minesweeper(QWidget *parent = 0);
	virtual ~minesweeper();

private:
	
	int times = 0;
	int mine = 10;
	int disCountMine = 0;
	int sizeX = 9;
	int sizeY = 9;

	cell *tablero[100][100];
	QGridLayout *grid = nullptr;
	QTimer *timer = nullptr;
	QLCDNumber *lcd1 = nullptr;
	QLCDNumber *lcd2 = nullptr;
	QPushButton *button = nullptr;
	QWidget *window = nullptr;

	void makeBoard();
	/*!
	 * Delete all the current board game and call the gerneration
	 * 
	 */
	void deleteBoard();

	/*!
	 * Delete the current board game and allow to modify the next size of the board game
	 * 
	 * \param Define the sizeX of the board
	 * \param Define the sizeY of the board
	 * \param Define the mines on the board
	 */
	void deleteBoard(int ,int ,int);

	void generateMine();
	void generateNumbers();
	void generateToolBar();
	void generateWindowSize();
	void generateWidgets();
	void generateWindowStructure();

	/*!
	* Calculate how many booms there are around one cell
	*
	* \param x is the coord X in the board
	* \param y is the coord Y in the board
	* \return How many booms there are around the cell
	*/
	int detectorAroundCell(int, int);

	/*!
	* To control the mouse events (clicks on the board)
	*
	* \param watched -->object observed
	* \param event --> event observed
	* \return if the event should be repeaded or not
	*/
	bool eventFilter(QObject *, QEvent *);

private slots:

	/*!
	* Signal used for counter in the minesweeper
	*
	*/
	void counter();
	/*!
	* Restard the game
	*
	*/
	void handleButton();

	/*!
	 * Generate a game with 9x9 with 10 booms
	 * 
	 */
	void easyGame();

	/*!
	* Generate a game with 16x16 with 40 booms
	*
	*/
	void intermidiateGame();

	/*!
	 * Generate a game with 16x30 with 99 booms
	 * 
	 */
	void expertGame();
};