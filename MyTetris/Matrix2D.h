/*
	Filename: Matrix2D.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate functionality of 2D matrix easily
	Comments: 
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/


#ifndef _MATRIX2D
#define _MATRIX2D

class Matrix2D
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	explicit Matrix2D(int order) : mOrder(order), mMaxIndex((order*order)-1)
	{
		//Create data for matrix
		mMatrixData = new int[mMaxIndex+1];

		if(mMatrixData)
		{
			//Init values
			int i = 0;
			for(i=0;i<=mMaxIndex;i++)
				mMatrixData[i] = 0;
		}
	}

	//Copy constructor
	Matrix2D(const Matrix2D &original)
	{
		_copyvalues(original);
	}

	~Matrix2D()
	{
		delete [] mMatrixData;
	}

	//----- Operators overloading -----
	//OPERATOR +
	Matrix2D& operator =(const Matrix2D &original)
	{
		//Check if original is not the same as this one..!
		if(this != &original)
		{
			//Make copy of values from original
			_copyvalues(original);
		}

		return(*this); //Return reference to allow multiple assignment
	}
	
	//----- GET/SET FUNCTIONS -----
	int GetOrder(){ return mOrder; };			//Get order of matrix
	int GetValue(int x, int y);					//Get x,y value
	void SetValue(int x, int y, int value);		//Set x,y value

	//----- OTHER FUNCTIONS -----
	void Reset(int value);						//Reset matrix to some value
	void Rotate(bool cw);						//Rotate Matrix
	bool IsZero();								//Checks if matrix is zero valued	

protected:
	int mOrder;				//Dimension of matrix (must be square)
	int mMaxIndex;			//Maximum index once matrix is created
	int *mMatrixData;		//Internal matrix data (1D array)

	//----- INTERNAL FUNCTIONS -----
	void _copyvalues(const Matrix2D &original);
};

#endif
