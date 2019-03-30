#pragma once
#include <new>

class Matrix
{
	public:
	class ProxyMatrix
			{	
				private:
					size_t cols;
				public:
					int* colNumber;
					
					ProxyMatrix (size_t cols)
						: cols (cols)

					{
						colNumber = new int [cols + 1];
					}
				
					int& operator [] (size_t colIndex)
					{
						if ( colIndex >= cols)
							throw std::out_of_range("");
						return colNumber [colIndex];
					}
	
					const int& operator [] (size_t colIndex) const
					{
						if ( colIndex >= cols)
							throw std::out_of_range("");
						return colNumber [colIndex];
					}
					
					~ProxyMatrix()
					{
						delete [] colNumber;
					}
				};
	private:
		const size_t rows;
		const size_t cols;
		ProxyMatrix* rowNumber;
	
	public:
		Matrix (const size_t ro, const size_t co)
		: rows(ro)
		, cols(co)
		{
			rowNumber = static_cast<ProxyMatrix*> (operator new [] ( rows*sizeof(ProxyMatrix) ));
			for ( int i = 0; i <= rows; i++)
				new (rowNumber + i) ProxyMatrix (cols);
		}	
		
		~Matrix ()
		{
			operator delete [] (rowNumber);
		}
		
		const size_t getRows () const
		{
			return rows;
		}
	
		const size_t getColumns () const
		{
			return cols;
		}	
	
		ProxyMatrix& operator [] (size_t rowIndex) 
		{
			if(rowIndex >= rows)
				throw std::out_of_range("");
			return rowNumber [rowIndex];
		}
		
		const ProxyMatrix& operator [] (size_t rowIndex) const
		{
			if(rowIndex >= rows)
				throw std::out_of_range("");
			return rowNumber [rowIndex];
		}

		Matrix& operator *= (const int& num)
		{
			for (size_t i = 0; i < rows; ++i)
				for (size_t j = 0; j < cols; ++j)
				{
					rowNumber[i].colNumber[j] = (rowNumber[i].colNumber[j] * num);
				}
			return *this;
		}

		bool operator == (const Matrix& other) const
		{	
			if (this == &other)
				return true;
		
			for (size_t i = 0; i < rows; ++i)
				for (size_t j = 0; j < cols; ++j)
					if (rowNumber[i].colNumber[j] != other.rowNumber[i].colNumber[j])
						return false;
			return true;
		}
		
		bool operator != (const Matrix& other) const
		{
			return !(*this == other);
		}
};
