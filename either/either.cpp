// copied from https://gist.github.com/3noch/6024523
#include <boost/optional.hpp>

/**
* Wraps any value with a context of Left for the Either class.
*
* By convention, Left represents some sort of less-desired condition.
*/
template<typename T>
class Left
{
public:
    Left(T v) : v_(v) {}
    const T & value() const throw () { return v_; }

private:
    T v_;
};


/**
* Equality comparisons for Left.
*/
template<typename L> bool operator==(const Left<L> & x, const Left<L> & y) { return x.value() == y.value(); }
template<typename L> bool operator!=(const Left<L> & x, const Left<L> & y) { return !(x == y); }

template<typename L> bool operator==(const Left<L> & x, const L & y) { return x.value() == y; }
template<typename L> bool operator!=(const Left<L> & x, const L & y) { return !(x == y); }
template<typename L> bool operator==(const L & x, const Left<L> & y) { return x == y.value(); }
template<typename L> bool operator!=(const L & x, const Left<L> & y) { return !(x == y); }


/**
* Wraps any value with a context of Right for the Either class.
*
* By convention, Right represents some sort of good condition.
*/
template<typename T>
class Right
{
public:
    Right(T v) : v_(v) {}
    const T & value() const throw () { return v_; }

private:
    T v_;
};


/**
* Equality comparisons for Right.
*/
template<typename R> bool operator==(const Right<R> & x, const Right<R> & y) { return x.value() == y.value(); }
template<typename R> bool operator!=(const Right<R> & x, const Right<R> & y) { return !(x == y); }

template<typename R> bool operator==(const Right<R> & x, const R & y) { return x.value() == y; }
template<typename R> bool operator!=(const Right<R> & x, const R & y) { return !(x == y); }
template<typename R> bool operator==(const R & x, const Right<R> & y) { return x == y.value(); }
template<typename R> bool operator!=(const R & x, const Right<R> & y) { return !(x == y); }


/**
* Like Haskell's Either data type, wraps a value as either a Left or Right.
*/
template<typename L, typename R>
class Either
{
public:
    typedef L LeftType;
    typedef R RightType;

    Either(R right)        : right_(right) {}
    Either(Left<L> left)   : left_(left.value()) {}
    Either(Right<R> right) : right_(right.value()) {}

    Either(bool useRight, L left, R right)
    {
        if (useRight) { right_ = right; }
        else          { left_ = left; }
    }

    bool isLeft()  const throw () { return (bool) left_; }
    bool isRight() const throw () { return (bool) right_; }

    const R & operator*() const  { return *right_; }
    const R & right()     const  { return *right_; }
    const L & left()      const  { return *left_; }
    const R * operator->() const { return &*right_; }

    operator bool() const throw () { return isRight(); }

private:
    boost::optional<L> left_;
    boost::optional<R> right_;
};


/**
* Equality comparisons for Either.
*/
template<typename L, typename R> bool operator==(const Either<L, R> & x, const Either<L, R> & y)
{
    if ( x &&  y) { return *x == *y; }
    if (!x && !y) { return x.left() == y.left(); }
    return false;
}
template<typename L, typename R> bool operator!=(const Either<L, R> & x, const Either<L, R> & y) { return !(x == y); }

template<typename L, typename R> bool operator==(const Either<L, R> & x, const R & y) { return x == Either<L, R>(y); }
template<typename L, typename R> bool operator!=(const Either<L, R> & x, const R & y) { return !(x == y); }
template<typename L, typename R> bool operator==(const R & x, const Either<L, R> & y) { return Either<L, R>(x) == y; }
template<typename L, typename R> bool operator!=(const R & x, const Either<L, R> & y) { return !(x == y); }

template<typename L, typename R> bool operator==(const Right<R> & x, const Either<L, R> & y) { return Either<L, R>(x) == y; }
template<typename L, typename R> bool operator!=(const Right<R> & x, const Either<L, R> & y) { return !(x == y); }
template<typename L, typename R> bool operator==(const Either<L, R> & x, const Right<R> & y) { return x == Either<L, R>(y); }
template<typename L, typename R> bool operator!=(const Either<L, R> & x, const Right<R> & y) { return !(x == y); }

template<typename L, typename R> bool operator==(const Left<L> & x, const Either<L, R> & y) { return Either<L, R>(x) == y; }
template<typename L, typename R> bool operator!=(const Left<L> & x, const Either<L, R> & y) { return !(x == y); }
template<typename L, typename R> bool operator==(const Either<L, R> & x, const Left<L> & y) { return x == Either<L, R>(y); }
template<typename L, typename R> bool operator!=(const Either<L, R> & x, const Left<L> & y) { return !(x == y); }

template<typename T> bool operator==(const Right<T> & x, const Left<T> & y) { return false; }
template<typename T> bool operator!=(const Right<T> & x, const Left<T> & y) { return !(x == y); }
template<typename T> bool operator==(const Left<T> & x, const Right<T> & y) { return false; }
template<typename T> bool operator!=(const Left<T> & x, const Right<T> & y) { return !(x == y); }
