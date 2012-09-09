/*
	Filename: Matrix2D.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate functionality of 2D matrix easily
	Comments: 
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "Matrix2D.h"

//Get x,y value
int Matrix2D::GetValue(int x, int y)	
{
	//Conversion of coordinates:
	//Real coordinate is linear, so inside linear array:
	//index =  x * mOrder + y
	return(mMatrixData[x*mOrder + y]);
}
	

//Set x,y value
void Matrix2D::SetValue(int x, int y, int value)
{
	//Conversion of coordinates:
	//Real coordinate is linear, so inside linear array:
	//index =  x * mOrder + y
	if((x < mOrder)
		&&
		(y < mOrder)
		)
	{
		mMatrixData[x*mOrder + y] = value;
	}
}

//Rotates a sizexsize SQUARE MATRIX clockwise or counterclockwise
void Matrix2D::Rotate(bool cw)
{
	//Rotation of the matrix stored
	//Note that matrix (2d) is stored linearly 
	//in an array, so indexing is quite different

	//Local variables
	int i;
	int j;
	int *tempmatrix = new int[mMaxIndex+1];
		
	//IF - ROTATION CLOCKWISE / COUNTERCLOCKWISE
	if(cw)
	{
		//Clockwise rotation
		for(i=0;i<mOrder;i++)
		{
			for(j=0;j<mOrder;j++)
			{
				//Linear access to memory; in 2d arrays would be:
				//tempmatrix[j][mOrder-i-1] = Matrix[i][j];
				//Linear access is coord i = mOrder * coord i / coord j = coord j
				//Sum both coords and we have real position!!
				tempmatrix[(mOrder*j)+(mOrder-i-1)] = mMatrixData[(mOrder*i)+j];
			}
		}
		
	}
	else //ELSE - ROTATION CLOCKWISE / COUNTERCLOCKWISE
	{
		//Counterclockwise rotation
		for(i=0;i<mOrder;i++)
		{
			for(j=0;j<mOrder;j++)
			{
				//Linear access to memory; in 2d arrays would be:
				//tempmatrix[mOrder-j-1][i] = Matrix[i][j];
				//Linear access is coord i = mOrder * coord i / coord j = coord j
				//Sum both coords and we have real position!!
				tempmatrix[(mOrder*(mOrder-j-1)) + i] = mMatrixData[(mOrder*i) + j];
			}
		}
	}//END IF - ROTATION CLOCKWISE / COUNTERCLOCKWISE

	//Copy results to original matrix

	for(i=0;i<mOrder;i++)
	{
		for(j=0;j<mOrder;j++)
		{
			//Linear access to memory; in 2d arrays would be:
			//Matrix[i][j] = tempmatrix[i][j];
			//Linear access is coord i = mOrder * coordi / coord j = coordj
			//Sum both coords and we have real position!!
			mMatrixData[(mOrder*i) + j] = tempmatrix[(mOrder*i) + j]; 
		}
	}

	//Free dynamically allocated memory
	delete [] tempmatrix;
}

//Reset to some value
void Matrix2D::Reset(int value)
{
	//Local variables
	int i,j;

	for(i=0;i<mOrder;i++)
	{
		for(j=0;j<mOrder;j++)
		{
			mMatrixData[i*mOrder + j] = value;
		}
	}
}

//Checks if matrix is zero valued	
bool Matrix2D::IsZero()
{
	//Check zero-valued everywhere
	for(int i = 0; i<=mMaxIndex ; i++)
	{
		if(mMatrixData[i] != 0)
			return(false);
	}

	return (true);
}

//Copy values from another MATRIX2D STRUCTURE (for copy constructor and assignment operator)
void Matrix2D::_copyvalues(const Matrix2D &original)
{

	mMaxIndex = original.mMaxIndex;
	mOrder = original.mOrder;
		
	//Deallocate dynamic memory used before
	if(mMatrixData)
		delete[] mMatrixData;

	mMatrixData = new int[original.mMaxIndex+1];
	if(mMatrixData)
	{
		//Init values
		int i = 0;
		for(i=0;i<=mMaxIndex;i++)
		{
			mMatrixData[i] = original.mMatrixData[i];
		}
	}
}