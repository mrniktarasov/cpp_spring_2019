class Matrix
{
	public:
	class proxyMatrix
		{	
			private:
				int* array1;
				size_t co;
			public:
				friend class Matrix;
				proxyMatrix () 
          			{
				}
				
				int& operator [] (size_t colNumber)
				{
					if ( colNumber >= co)
						throw std::out_of_range("");
					return array1[colNumber];
				}
	
				const int& operator [] (size_t colNumber) const
				{
					if ( colNumber >= co)
						throw std::out_of_range("");
					return array1[colNumber];
				};
	private:
		const size_t rows;
		const size_t cols;
		proxyMatrix* array2;
	
	public:
		Matrix (const size_t ro, const size_t co)
		: rows(ro)
		, cols(co)
		{
			array2 = new proxyMatrix [rows+1];
			for ( size_t i = 0; i <= rows; i++)
			{
				array2[i].co = cols;
				array2[i].array1 = new int [cols+1];
			}
		}
		
		~Matrix ()
		{	
			for( int i = 0; i <= rows; i++)
				delete[] array2[i].array1;
			delete[] array2;
		}
		
		const size_t getRows () const
		{
			const size_t temp = rows;
			return temp;
		}
	
		const size_t getColumns () const
		{
			const size_t temp = cols;
			return temp;
		}	
	
		proxyMatrix& operator [] (size_t rowNumber) 
		{
			if(rowNumber >= rows)
				throw std::out_of_range("");
			return array2[rowNumber];
		}
		
		const proxyMatrix& operator [] (size_t rowNumber) const
		{
			if(rowNumber >= rows)
				throw std::out_of_range("");
			return array2[rowNumber];
		}

		void operator *= (const int& num)
		{
			for (size_t i = 0; i < rows; ++i)
				for (size_t j = 0; j < cols; ++j)
				{
					array2[i].array1[j] = (array2[i].array1[j] * num);
				}
		}

		bool operator == (const Matrix& other) const
		{	
			if (this == &other)
				return true;
		
			for (size_t i = 0; i < rows; ++i)
				for (size_t j = 0; j < cols; ++j)
					if (array2[i].array1[j] != other.array2[i].array1[j])
						return false;
			return true;
		}
		
		bool operator != (const Matrix& other) const
		{
			return !(*this == other);
		}
};

