




#ifndef __LA_VECTOR_H
#define __LA_VECTOR_H




#ifndef DJC_COROPA_LIBALGEBRAH_SEEN
typedef unsigned DEG;
#endif

#include <type_traits>


template <bool b, typename T = void>
using enable_if_t = typename std::enable_if<b, T>::type;
namespace our_vector {


class our_vector_iterator 
{

};

class const_our_vector_iterator
{

};

template <typename BASIS>
class sparse_vector_interface
{
	/// Import of the rational type from the BASIS class.
	typedef typename BASIS::RATIONAL RATIONAL;
	/// Import of the KEY type from the MAP class.
	typedef typename BASIS::MAP::key_type KEY;
	/// Import of the SCALAR type from the MAP class.
	typedef typename BASIS::MAP::mapped_type SCALAR;
	/// Import of the iterator type from the MAP type.
	typedef our_vector_iterator iterator;
	/// Import of the KEY constant iterator type from the MAP type.
	typedef const_our_vector_iterator const_iterator;

	/// Import of Const_Iterator to beginning of the sparse vector
	using MAP::begin;
	/// Import of Const_Iterator to end of the sparse vector
	using MAP::end;
	/// Import of find a KEY in the sparse vector
	using MAP::find;


	/// Import of insert a KEY SCALAR into the sparse vector
	using MAP::insert;
	/// Import of erase a KEY from the sparse vector
	using MAP::erase;
	/// Import of set a KEY with a given SCALAR 
	using MAP::operator [];

	/// Import of set this instance to the zero instance
	using MAP::clear;
	/// Import empty()
	using MAP::empty;

	/// Import size()
	using MAP::size;

	/// Swap the vector instance controlled by *this with the one in the RHS
	void swap(sparse_vector & rhs);


    sparse_vector(void) {}

	/// Copy constructor. 
	sparse_vector(const sparse_vector& v);

	/// Unidimensional constructor.
	/**
	* Constructs a sparse_vector corresponding the unique basis
	* element k with coefficient s (+1 by default).
	*/
	explicit sparse_vector(const KEY& k, const SCALAR& s = one);

    // Binary operations + others

};

template<typename BASIS>
class vector_constants
{
public:
	static BASIS basis;
	static typename BASIS::SCALAR zero;
	static typename BASIS::SCALAR one;
	static typename BASIS::SCALAR mone;
};

// Initialise static members
template<class BASIS>
BASIS vector_constants<BASIS>::basis;

template<class BASIS>
const typename BASIS::SCALAR vector_constants<BASIS>::one(+1);

template<class BASIS>
const typename BASIS::SCALAR vector_constants<BASIS>::zero(0);

template<class BASIS>
const typename BASIS::SCALAR vector_constants<BASIS>::mone(-1);


#define DENSE_TRANSITION 5
#define __DECLARE_BINARY_OPERATOR(T1, NEWOP, OLDOP, T2) \
	T1 operator NEWOP(const T2& rhs) const \
	{ T1 result(*this); return result OLDOP rhs; }

#define __DECLARE_UNARY_OPERATOR(NEWT, NEWOP, OLDOP, OLDT) \
	NEWT operator NEWOP(void) const \
	{ return OLDT::operator OLDOP (); }


template <typename BASIS>
class sparse_vector;



template <typename BASIS, DEG num_elts>
class dense_vector : public vector_constants<BASIS>
{
public:
	typedef typename BASIS::KEY KEY;
	typedef typename BASIS::SCALAR SCALAR:

private:
	std::array<KEY, num_elts> _keys;
	std::array<SCALAR, num_lets> _buf;

	static const dimension = num_elts;

public:

	/// Default constructor
	dense_vector()
	{
		_buf.fill(zero);
	}

	/// Copy constructor
	dense_vector(const vector& v) : _keys(v._keys), _buf(v._buf)
		{}

	inline void swap(vector &rhs)
	{
		_buf.swap(rhs._buf);
	}

	/// Remove all entries from the vector
	inline void clear()
	{
		_buf.fill(zero);
	}

	/// Get the size of the 
	inline size_t size() const
	{
		size_t i = 0;
		for (auto item : _buf) {
			if (zero != item) ++i;
		}
		return i;
	}

	/// Get whether the vector is empty
	inline bool empty() const 
	{
		for (auto i : _buf) {
			if (zero != item) return true;
		}
		return false;
	}

	// Insert is a problem because it has a number of different
	// overloads. Perhaps we could do a template function overload
	// to make it 
	auto insert(const KEY k, SCALAR v)
	{
		return DENSE::insert(k, v);
	}

	inline SCALAR & operator[](const KEY k)
    {
		return _buf[k];
    }

    inline vector operator-(void) const
	{
		vector result;
		size_t idx = 0;
		for (auto item : _buf) {
			result[idx++] = -item;
		}
	}

	/// Multiplies the instance with scalar s.
	inline vector& operator*=(const SCALAR& s)
	{
		for (auto item : _buf)
			item *= s;
	}

	/// Binary version of operator*=()
	inline __DECLARE_BINARY_OPERATOR(vector, *, *=, SCALAR);

	/// Divides the instance by scalar s.
	inline vector& operator/=(const RATIONAL& s)
	{
		for (auto item ; _buf)
			item /= s;
	}

	/// Binary instance of  operator/=()
	inline __DECLARE_BINARY_OPERATOR(vector, / , /=, RATIONAL);

	/// Adds a sparse_vector to the instance.
	inline vector& operator+=(const vector& rhs)
    {
		size_t i = 0;
		for (auto item : _buf)
			item += rhs[i++];
    }

	/// Binary version of  operator+=()
	inline __DECLARE_BINARY_OPERATOR(vector, +, +=, vector);

	/// Subtracts a sparse_vector to the instance.
	inline vector& operator-=(const vector& rhs)
	{
		size_t i = 0;
		for (auto item : _buf)
			item -= rhs[i++];
    }
	
	/// Binary version of  operator-=()
	inline __DECLARE_BINARY_OPERATOR(vector, -, -=, vector);
		
	/// Where SCA admits an order forms the min of two sparse vectors
	inline vector& operator&=(const vector& rhs)
	{
		return *this
    }
	/// Binary version of  operator&=()
	inline __DECLARE_BINARY_OPERATOR(vector, &, &=, vector);

	/// Where SCA admits an order forms the max of two sparse vectors
	inline vector& operator|=(const vector& rhs)
	{
		return *this
    }

	/// Binary version of  operator|=()
	inline __DECLARE_BINARY_OPERATOR(vector, | , |=, vector);
}


template <typename BASIS, DEG TRANSITION = DENSE_TRANSITION>
class vector 
	: sparse_vector<BASIS>, 
	  dense_vector<BASIS, TRANSITION>, 
	  public vector_constants<BASIS>
{
public:
	typedef typename BASIS::KEY KEY;

private:
	typedef sparse_vector<BASIS> SPARSE;
	typedef dense_vector<BASIS> DENSE;

	explicit vector(const DENSE&& dpart, const SPARSE&& spart)
		: DENSE(dpart), SPARSE(spart)
		{}

	static const KEY dense_transition = TRANSITION;

public:

	typedef typename BASIS::SCALAR SCALAR;
	typedef typename BASIS::RATIONAL RATIONAL;
	typedef typename BASIS::KEY KEY;

	/// Default constructor
	vector(void) : DENSE(), SPARSE() {}

	vector(const sparse_vector &v) : DENSE(), SPARSE()
	{
		for (auto item : v) {
			(*this)[item.first] = item.second;
		}
	}

	explicit vector(const KEY& k, const SCALAR& s = one) :
		DENSE(), SPARSE()
	{
		if (zero != s)
			(*this)[k] = s;
	}

	inline void swap(vector &rhs)
	{
		DENSE::swap((DENSE&) rhs);
		SPARSE::swap((SPARSE&) rhs);
	}

	/// Remove all entries from the vector
	inline void clear()
	{
		DENSE::clear();
		SPARSE::clear();
	}

	/// Get the size of the 
	inline size_t size() const
	{
		return DENSE::size() + SPARSE::size();
	}

	/// Get whether the vector is empty
	inline bool empty() const 
	{
		return DENSE::empty() && SPARSE::empty();
	}

	// Insert is a problem because it has a number of different
	// overloads. Perhaps we could do a template function overload
	// to make it 
	auto insert(const KEY k, SCALAR v)
	{
		if (k <= dense_transition)
			return DENSE::insert(k, v);
		else
			return SPARSE::insert(k, v);
	}

	inline SCALAR & operator[](const KEY k)
    {
		if (k <= dense_transition)
			return DENSE::operator[](k);
		else
			return SPARSE::operator[](k);
    }

    inline vector operator-(void) const
	{
		if (empty())
			return *this;
		
		return vector(DENSE::operator-(), SPARSE::operator-());

	}

	/// Multiplies the instance with scalar s.
	inline vector& operator*=(const SCALAR& s)
	{
		DENSE::operator*=(s);
		SPARSE::operator*=(s);
		return *this;
	}

	/// Binary version of operator*=()
	inline __DECLARE_BINARY_OPERATOR(vector, *, *=, SCALAR);

	/// Divides the instance by scalar s.
	inline vector& operator/=(const RATIONAL& s);
	/// Binary instance of  operator/=()
	inline __DECLARE_BINARY_OPERATOR(vector, / , /=, RATIONAL);

	/// Adds a sparse_vector to the instance.
	inline vector& operator+=(const vector& rhs)
    {
		DENSE::operator+=((const DENSE&) rhs);
		SPARSE::operator+=((const SPARSE&) rhs);
		return *this
    }

	/// Binary version of  operator+=()
	inline __DECLARE_BINARY_OPERATOR(vector, +, +=, vector);

	/// Subtracts a sparse_vector to the instance.
	inline vector& operator-=(const vector& rhs)
	{
		DENSE::operator-=((const DENSE&) rhs);
		SPARSE::operator-=((const SPARSE&) rhs);
		return *this
    }
	/// Binary version of  operator-=()
	inline __DECLARE_BINARY_OPERATOR(vector, -, -=, vector);
		
	/// Where SCA admits an order forms the min of two sparse vectors
	inline vector& operator&=(const vector& rhs)
	{
		// this implementation needs some thought.
		DENSE::operator&=((const DENSE&) rhs);
		SPARSE::operator&=((const SPARSE&) rhs);
		return *this
    }

	/// Binary version of  operator&=()
	inline __DECLARE_BINARY_OPERATOR(vector, &, &=, vector);

	/// Where SCA admits an order forms the max of two sparse vectors
	inline vector& operator|=(const vector& rhs)
	{
		// this implementation needs some thought.
		DENSE::operator|=((const DENSE&) rhs);
		SPARSE::operator|=((const SPARSE&) rhs);
		return *this
    }

	/// Binary version of  operator|=()
	inline __DECLARE_BINARY_OPERATOR(vector, | , |=, vector);


};







} // end namespace
#endif