#ifndef _QUANTITY_H
#define _QUANTITY_H

#include <cassert>
#include <cmath>
#include <iostream>

namespace quantity {
#if 0
} // avoids autoindent of namespace
#endif



// CLASSES

/**
 * Template class storing a vector-valued physical quantity.
 *
 * The value of the quantity is stored in SI units (kilograms, meters and
 * seconds). All operations are checked for correct dimensions at compile-time.
 * See also: http://en.wikipedia.org/wiki/Dimensional_analysis
 *
 * @tparam[T] The basic data type, e.g. double or float.
 * @tparam[N] The dimensionality of the data vector, typically 3.
 * @tparam[MASS] The exponent of kilograms in the physical unit.
 * @tparam[LENGTH] The exponent of meters in the physical unit.
 * @tparam[TIME] The exponent of seconds in the physical unit.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME>
class Quantity {
	public:
		typedef T value_type; //< Basic data type.

		/**
		 * Zero-value constructor.
		 */
		Quantity(): _value() {}

		/**
		 * Copy constructor.
		 */
		template <typename U> Quantity(const Quantity<U, N, MASS, LENGTH, TIME> &other) {
			for (size_t idx = 0; idx < N; ++idx)
				_value[idx] = other[idx];
		}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
		/**
		 * Constructor from a list of numbers.
		 *
		 * Example: Quantity<double, 3, 0, 0, 0> q = {1.0, 2.0, 3.0};
		 * This is only available if the C++0x standard is activated in gcc by
		 * using the -std=c++0x flag.
		 */
		template <typename U> Quantity(std::initializer_list<U> items) {
			assert (items.size() == N);
			const U *it = items.begin();
			for (size_t idx = 0; idx < N; ++it, ++idx)
				_value[idx] = *it;
		}
#endif

		/**
		 * Read-write access to single elements of a vector-valued quantity.
		 */
		Quantity<T, 1, MASS, LENGTH, TIME> &operator[](size_t idx) {
			assert(idx < N);
			return _value[idx];
		}

		/**
		 * Read-only access to single elements of a vector-valued quantity.
		 */
		const Quantity<T, 1, MASS, LENGTH, TIME> &operator[](size_t idx) const {
			assert(idx < N);
			return _value[idx];
		}

		/**
		 * Unary plus operator.
		 */
		const Quantity<T, N, MASS, LENGTH, TIME> &operator+() const {
			return *this;
		}

		/**
		 * Unary minux operator.
		 */
		Quantity<T, N, MASS, LENGTH, TIME> operator-() const {
			return -1.0 * *this;
		}

		/**
		 * In-place addition of another quantity of same dimensions.
		 */
		template <typename U> Quantity<T, N, MASS, LENGTH, TIME> &operator+=(const Quantity<U, N, MASS, LENGTH, TIME> &other) {
			for (size_t idx = 0; idx < N; ++idx)
				_value[idx] += other[idx];
			return *this;
		}

		/**
		 * In-place subtraction of another quantity of same dimensions.
		 */
		template <typename U> Quantity<T, N, MASS, LENGTH, TIME> &operator-=(const Quantity<U, N, MASS, LENGTH, TIME> &other) {
			for (size_t idx = 0; idx < N; ++idx)
				_value[idx] -= other[idx];
			return *this;
		}

		/**
		 * In-place multiplication with a dimensionless scalar or scalar quantity.
		 */
		template <typename U> Quantity<T, N, MASS, LENGTH, TIME> &operator*=(const U &other) {
			for (size_t idx = 0; idx < N; ++idx)
				_value[idx] *= other;
			return *this;
		}

		/**
		 * In-place division by a dimensionless scalar or scalar quantity.
		 */
		template <typename U> Quantity<T, N, MASS, LENGTH, TIME> &operator/=(const U &other) {
			for (size_t idx = 0; idx < N; ++idx)
				_value[idx] /= other;
			return *this;
		}

		/**
		 * Equality operator for vectorial quantities.
		 *
		 * Vectorial quantities are considered equal if all their components are equal.
		 */
		bool operator==(const Quantity<T, N, MASS, LENGTH, TIME> &other) {
			for (size_t idx = 0; idx < N; ++idx)
				if (!(_value[idx] == other._value[idx]))
					return false;
			return true;
		}

		/**
		 * Inequality operator for vectorial quantities.
		 *
		 * Vectorial quantities are considered unequal if they differ in at least one component.
		 */
		bool operator!=(const Quantity<T, N, MASS, LENGTH, TIME> &other) {
			for (size_t idx = 0; idx < N; ++idx)
				if (_value[idx] != other._value[idx])
					return true;
			return false;
		}

	private:
		Quantity<T, 1, MASS, LENGTH, TIME> _value[N];
};

/**
 * Template specialization for scalar physical quantities.
 *
 * The value of the quantity is stored in SI units (kilograms, meters and
 * seconds). If you avoid using the value() member function, all operations
 * are checked for correct dimensions at compile-time. See also:
 * http://en.wikipedia.org/wiki/Dimensional_analysis
 *
 * @tparam[T] The basic data type, e.g. double or float.
 * @tparam[MASS] The exponent of kilograms in the physical unit.
 * @tparam[LENGTH] The exponent of meters in the physical unit.
 * @tparam[TIME] The exponent of seconds in the physical unit.
 */
template <typename T, int MASS, int LENGTH, int TIME>
class Quantity<T, 1, MASS, LENGTH, TIME> {
	public:
		typedef T value_type; //< Basic data type.

		/**
		 * Zero-value constructor.
		 */
		Quantity(): _value() {}

		/**
		 * Copy constructor.
		 */
		template <typename U> Quantity(const Quantity<U, 1, MASS, LENGTH, TIME> &other): _value(other._value) {}

		/**
		 * Explicit construction from a scalar.
		 *
		 * Use with caution, as this operation cannot be checked for dimensional correctness.
		 */
		template <typename U> explicit Quantity(const U &value): _value(value) {}

		/*
		 * Read-write access to the stored scalar value.
		 *
		 * Use with caution, as this operation cannot be checked for dimensional correctness.
		 */
		T &value() { return _value; }

		/**
		 * Read-only access to the stored scalar value.
		 *
		 * Use with caution, as this operation cannot be checked for dimensional correctness.
		 */
		const T &value() const { return _value; }

		/**
		 * Unary plus operator.
		 */
		const Quantity<T, 1, MASS, LENGTH, TIME> &operator+() const {
			return *this;
		}

		/**
		 * Unary minux operator.
		 */
		Quantity<T, 1, MASS, LENGTH, TIME> operator-() const {
			return -1.0 * *this;
		}

		/**
		 * In-place addition of another quantity of same dimensions.
		 */
		template <typename U> Quantity<T, 1, MASS, LENGTH, TIME> &operator+=(const Quantity<U, 1, MASS, LENGTH, TIME> &other) {
			_value += other._value;
			return *this;
		}

		/**
		 * In-place subtraction of another quantity of same dimensions.
		 */
		template <typename U> Quantity<T, 1, MASS, LENGTH, TIME> &operator-=(const Quantity<U, 1, MASS, LENGTH, TIME> &other) {
			_value -= other._value;
			return *this;
		}

		/**
		 * In-place multiplication with a dimensionless scalar or scalar quantity.
		 */
		template <typename U> Quantity<T, 1, MASS, LENGTH, TIME> &operator*=(const U &other) {
			_value *= other;
			return *this;
		}

		/**
		 * In-place division by a dimensionless scalar or scalar quantity.
		 */
		template <typename U> Quantity<T, 1, MASS, LENGTH, TIME> &operator/=(const U &other) {
			_value /= other;
			return *this;
		}

		/**
		 * Less-than operator for scalar quantities.
		 */
		bool operator<(const Quantity<T, 1, MASS, LENGTH, TIME> &other) {
			return _value < other._value;
		}

		/**
		 * Less-or-equal operator for scalar quantities.
		 */
		bool operator<=(const Quantity<T, 1, MASS, LENGTH, TIME> &other) {
			return _value <= other._value;
		}

		/**
		 * Greater-than operator for scalar quantities.
		 */
		bool operator>(const Quantity<T, 1, MASS, LENGTH, TIME> &other) {
			return _value > other._value;
		}

		/**
		 * Greater-or-equal operator for scalar quantities.
		 */
		bool operator>=(const Quantity<T, 1, MASS, LENGTH, TIME> &other) {
			return _value >= other._value;
		}

		/**
		 * Equality operator for scalar quantities.
		 *
		 * Scalar quantities of matching dimension are considered equal if their values are equal.
		 */
		bool operator==(const Quantity<T, 1, MASS, LENGTH, TIME> &other) {
			return _value == other._value;
		}

		/**
		 * Inequality operator for scalar quantities.
		 *
		 * Scalar quantities of matching dimension are considered unequal if their values are unequal.
		 */
		bool operator!=(const Quantity<T, 1, MASS, LENGTH, TIME> &other) {
			return _value != other._value;
		}

	private:
		T _value;
};

/**
 * Template specialization for dimensionless scalar physical quantities.
 *
 * @tparam[T] The basic data type, e.g. double or float.
 */
template <typename T>
class Quantity<T, 1, 0, 0, 0> {
	public:
		typedef T value_type; //< Basic data type.

		/**
		 * Zero-value constructor.
		 */
		Quantity(): _value() {}

		/**
		 * Copy constructor.
		 */
		template <typename U> Quantity(const Quantity<U, 1, 0, 0, 0> &other): _value(other._value) {}

		/**
		 * Explicit construction from a scalar.
		 */
		template <typename U> Quantity(const U &value): _value(value) {}

		/*
		 * Read-write access to the stored scalar value.
		 */
		T &value() { return _value; }

		/**
		 * Read-only access to the stored scalar value.
		 */
		const T &value() const { return _value; }

		/**
		 * Read-write type casting conversion to a dimensionless scalar.
		 */
		operator T&() { return _value; }

		/**
		 * Read-only type casting conversion to a dimensionless scalar.
		 */
		operator const T&() const { return _value; }

		/**
		 * Unary plus operator.
		 */
		const Quantity<T, 1, 0, 0, 0> &operator+() const {
			return *this;
		}

		/**
		 * Unary minux operator.
		 */
		Quantity<T, 1, 0, 0, 0> operator-() const {
			return -1.0 * *this;
		}

		/**
		 * In-place addition of another quantity of same dimensions.
		 */
		template <typename U> Quantity<T, 1, 0, 0, 0> &operator+=(const Quantity<U, 1, 0, 0, 0> &other) {
			_value += other._value;
			return *this;
		}

		/**
		 * In-place subtraction of another quantity of same dimensions.
		 */
		template <typename U> Quantity<T, 1, 0, 0, 0> &operator-=(const Quantity<U, 1, 0, 0, 0> &other) {
			_value -= other._value;
			return *this;
		}

		/**
		 * In-place multiplication with a dimensionless scalar or scalar quantity.
		 */
		template <typename U> Quantity<T, 1, 0, 0, 0> &operator*=(const U &other) {
			_value *= other;
			return *this;
		}

		/**
		 * In-place division by a dimensionless scalar or scalar quantity.
		 */
		template <typename U> Quantity<T, 1, 0, 0, 0> &operator/=(const U &other) {
			_value /= other;
			return *this;
		}

		/**
		 * Less-than operator for scalar quantities.
		 */
		bool operator<(const Quantity<T, 1, 0, 0, 0> &other) {
			return _value < other._value;
		}

		/**
		 * Less-or-equal operator for scalar quantities.
		 */
		bool operator<=(const Quantity<T, 1, 0, 0, 0> &other) {
			return _value <= other._value;
		}

		/**
		 * Greater-than operator for scalar quantities.
		 */
		bool operator>(const Quantity<T, 1, 0, 0, 0> &other) {
			return _value > other._value;
		}

		/**
		 * Greater-or-equal operator for scalar quantities.
		 */
		bool operator>=(const Quantity<T, 1, 0, 0, 0> &other) {
			return _value >= other._value;
		}

		/**
		 * Equality operator for scalar quantities.
		 *
		 * Scalar quantities of matching dimension are considered equal if their values are equal.
		 */
		bool operator==(const Quantity<T, 1, 0, 0, 0> &other) {
			return _value == other._value;
		}

		/**
		 * Inequality operator for scalar quantities.
		 *
		 * Scalar quantities of matching dimension are considered unequal if their values are unequal.
		 */
		bool operator!=(const Quantity<T, 1, 0, 0, 0> &other) {
			return _value != other._value;
		}


	private:
		T _value;
};



// OUTPUT OPERATORS

/**
 * Stream output for physical quantities, including dimensions.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME>
std::ostream &operator<<(std::ostream &o, const Quantity<T, N, MASS, LENGTH, TIME> &q) {
	o << "(" << q[0].value();
	for (size_t idx = 1; idx < N; ++idx)
		o << ", " << q[idx].value();
	o << ")";
	if (MASS != 0) {
		o << " kg";
		if (MASS != 1)
			o << "^" << MASS;
	}
	if (LENGTH != 0) {
		o << " m";
		if (LENGTH != 1)
			o << "^" << LENGTH;
	}
	if (TIME != 0) {
		o << " s";
		if (TIME != 1)
			o << "^" << TIME;
	}
	return o;
}

/**
 * Template specialization for scalar quantities.
 */
template <typename T, int MASS, int LENGTH, int TIME>
std::ostream &operator<<(std::ostream &o, const Quantity<T, 1, MASS, LENGTH, TIME> &q) {
	o << q.value();
	if (MASS != 0) {
		o << " kg";
		if (MASS != 1)
			o << "^" << MASS;
	}
	if (LENGTH != 0) {
		o << " m";
		if (LENGTH != 1)
			o << "^" << LENGTH;
	}
	if (TIME != 0) {
		o << " s";
		if (TIME != 1)
			o << "^" << TIME;
	}
	return o;
}



// OPERATORS

/**
 * Sum of two vectorial quantities.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME>
Quantity<T, N, MASS, LENGTH, TIME>
operator+(const Quantity<T, N, MASS, LENGTH, TIME> &q1, const Quantity<T, N, MASS, LENGTH, TIME> &q2) {
	Quantity<T, N, MASS, LENGTH, TIME> q(q1);
	q += q2;
	return q;
}

/**
 * Difference of two vectorial quantities.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME>
Quantity<T, N, MASS, LENGTH, TIME>
operator-(const Quantity<T, N, MASS, LENGTH, TIME> &q1, const Quantity<T, N, MASS, LENGTH, TIME> &q2) {
	Quantity<T, N, MASS, LENGTH, TIME> q(q1);
	q -= q2;
	return q;
}

/**
 * Product of a vectorial quantity with a dimensionless scalar.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME, typename U>
Quantity<T, N, MASS, LENGTH, TIME>
operator*(const Quantity<T, N, MASS, LENGTH, TIME> &q1, const U &v) {
	Quantity<T, N, MASS, LENGTH, TIME> q(q1);
	q *= v;
	return q;
}

/**
 * Product of a dimensionless scalar with a vectorial quantity.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME, typename U>
Quantity<T, N, MASS, LENGTH, TIME>
operator*(const U &v, const Quantity<T, N, MASS, LENGTH, TIME> &q1) {
	Quantity<T, N, MASS, LENGTH, TIME> q(q1);
	q *= v;
	return q;
}

/**
 * Product of two scalar quantities.
 */
template <typename T, int MASS1, int LENGTH1, int TIME1, int MASS2, int LENGTH2, int TIME2>
Quantity<T, 1, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2>
operator*(const Quantity<T, 1, MASS1, LENGTH1, TIME1> &q1, const Quantity<T, 1, MASS2, LENGTH2, TIME2> &q2) {
	Quantity<T, 1, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2> q(*reinterpret_cast<const Quantity<T, 1, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2>*>(&q1));
	q *= q2.value();
	return q;
}

/**
 * Product of a scalar quantity with a vectorial quantity.
 */
template <typename T, size_t N, int MASS1, int LENGTH1, int TIME1, int MASS2, int LENGTH2, int TIME2>
Quantity<T, N, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2>
operator*(const Quantity<T, 1, MASS1, LENGTH1, TIME1> &q1, const Quantity<T, N, MASS2, LENGTH2, TIME2> &q2) {
	Quantity<T, N, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2> q(*reinterpret_cast<const Quantity<T, N, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2>*>(&q2));
	q *= q1.value();
	return q;
}

/**
 * Product of a vectorial quantity with a scalar quantity.
 */
template <typename T, size_t N, int MASS1, int LENGTH1, int TIME1, int MASS2, int LENGTH2, int TIME2>
Quantity<T, N, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2>
operator*(const Quantity<T, N, MASS1, LENGTH1, TIME1> &q1, const Quantity<T, 1, MASS2, LENGTH2, TIME2> &q2) {
	Quantity<T, N, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2> q(*reinterpret_cast<const Quantity<T, N, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2>*>(&q1));
	q *= q2.value();
	return q;
}

/**
 * Quotient of a vectorial quantity and a dimensionless scalar.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME, typename U>
Quantity<T, N, MASS, LENGTH, TIME>
operator/(const Quantity<T, N, MASS, LENGTH, TIME> &q1, const U &v) {
	Quantity<T, N, MASS, LENGTH, TIME> q(q1);
	q /= v;
	return q;
}

/**
 * Quotient of two scalar quantities.
 */
template <typename T, int MASS1, int LENGTH1, int TIME1, int MASS2, int LENGTH2, int TIME2>
Quantity<T, 1, MASS1 - MASS2, LENGTH1 - LENGTH2, TIME1 - TIME2>
operator/(const Quantity<T, 1, MASS1, LENGTH1, TIME1> &q1, const Quantity<T, 1, MASS2, LENGTH2, TIME2> &q2) {
	Quantity<T, 1, MASS1 - MASS2, LENGTH1 - LENGTH2, TIME1 - TIME2> q(*reinterpret_cast<const Quantity<T, 1, MASS1 - MASS2, LENGTH1 - LENGTH2, TIME1 - TIME2>*>(&q1));
	q /= q2.value();
	return q;
}

/**
 * Quotient of a vectorial quantity and a scalar quantity.
 */
template <typename T, size_t N, int MASS1, int LENGTH1, int TIME1, int MASS2, int LENGTH2, int TIME2>
Quantity<T, N, MASS1 - MASS2, LENGTH1 - LENGTH2, TIME1 - TIME2>
operator/(const Quantity<T, N, MASS1, LENGTH1, TIME1> &q1, const Quantity<T, 1, MASS2, LENGTH2, TIME2> &q2) {
	Quantity<T, N, MASS1 - MASS2, LENGTH1 - LENGTH2, TIME1 - TIME2> q(*reinterpret_cast<const Quantity<T, N, MASS1 - MASS2, LENGTH1 - LENGTH2, TIME1 - TIME2>*>(&q1));
	q /= q2.value();
	return q;
}

/**
 * Quotient of a dimensionless scalar and a scalar quantity.
 */
template <typename U, typename T, int MASS, int LENGTH, int TIME>
Quantity<T, 1, -MASS, -LENGTH, -TIME>
operator/(const U &v, const Quantity<T, 1, MASS, LENGTH, TIME> &q1) {
	Quantity<T, 1, 0, 0, 0> q(v);
	return q / q1;
}



// FUNCTIONS

/**
 * Cross product of two three-dimensional quantities.
 */
template <typename T, int MASS1, int LENGTH1, int TIME1, int MASS2, int LENGTH2, int TIME2>
Quantity<T, 3, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2>
cross(const Quantity<T, 3, MASS1, LENGTH1, TIME1> &q1, const Quantity<T, 3, MASS2, LENGTH2, TIME2> &q2) {
	Quantity<T, 3, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2> q;
	q[0] = q1[1] * q2[2] - q1[2] * q2[1];
	q[1] = q1[2] * q2[0] - q1[0] * q2[2];
	q[2] = q1[0] * q2[1] - q1[1] * q2[0];
	return q;
}

/**
 * Dot product of two vectorial quantities.
 */
template <typename T, size_t N, int MASS1, int LENGTH1, int TIME1, int MASS2, int LENGTH2, int TIME2>
Quantity<T, 1, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2>
dot(const Quantity<T, N, MASS1, LENGTH1, TIME1> &q1, const Quantity<T, N, MASS2, LENGTH2, TIME2> &q2) {
	Quantity<T, 1, MASS1 + MASS2, LENGTH1 + LENGTH2, TIME1 + TIME2> q;
	for (size_t idx = 0; idx < N; ++idx)
		q += q1[idx] * q2[idx];
	return q;
}

/**
 * Squared norm of a vectorial quantity.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME>
Quantity<T, 1, 2 * MASS, 2 * LENGTH, 2 * TIME>
squared_norm(const Quantity<T, N, MASS, LENGTH, TIME> &q) {
	return dot(q, q);
}

/**
 * Norm of a vectorial quantity.
 */
template <typename T, size_t N, int MASS, int LENGTH, int TIME>
Quantity<T, 1, MASS, LENGTH, TIME>
norm(const Quantity<T, N, MASS, LENGTH, TIME> &q1) {
	return Quantity<T, 1, MASS, LENGTH, TIME>(std::sqrt(squared_norm(q1).value()));
}

/**
 * Compute a normalized copy of q1.
 */
template<typename T, size_t N, int MASS, int LENGTH, int TIME>
Quantity<T, N, 0, 0, 0>
normalized(const Quantity<T, N, MASS, LENGTH, TIME> &q1) {
	return q1 / norm(q1);
}

/**
 * Compute an approximately normalized copy of q1.
 *
 * This function is differentiable around q1 == 0.
 */
template<typename T, size_t N, int MASS, int LENGTH, int TIME>
Quantity<T, N, 0, 0, 0>
normalized(const Quantity<T, N, MASS, LENGTH, TIME> &q1, const Quantity<T, 1, MASS, LENGTH, TIME> &q2, double exponent) {
	if (q2.value() == 0) return normalized(q1);
	Quantity<T, 1, MASS, LENGTH, TIME> length = norm(q1);
	T relative_length = length / q2;
	return q1 * pow(relative_length, exponent + 1) / (length * (1.0 + pow(relative_length, exponent)));
}

/**
 * Helper class for dimensionality checking in sqrt().
 */
template <bool dimensions_okay>
struct sqrt_helper_class {
};

/**
 * Specialization for correct dimensions.
 */
template <>
struct sqrt_helper_class<true> {
	/**
	 * Actual implementation of sqrt().
	 */
	template <typename T, int MASS, int LENGTH, int TIME>
		static Quantity<T, 1, MASS / 2, LENGTH / 2, TIME / 2>
		sqrt(const Quantity<T, 1, MASS, LENGTH, TIME> &q1) {
			return Quantity<T, 1, MASS / 2, LENGTH / 2, TIME / 2>(std::sqrt(q1.value()));
		}
};

/**
 * Square root of a scalar quantity.
 */
template <typename T, int MASS, int LENGTH, int TIME>
Quantity<T, 1, MASS / 2, LENGTH / 2, TIME / 2>
sqrt(const Quantity<T, 1, MASS, LENGTH, TIME> &q1) {
	return sqrt_helper_class<MASS % 2 == 0 && LENGTH % 2 == 0 && TIME % 2 == 0>::sqrt(q1); // Provoke compiler error for bad dimensions.
}

/**
 * P-th power of a scalar quantity.
 *
 * Example:
 * Length l;
 * Area a = pow<2>(l);
 *
 * @tparam[P] The exponent.
 * @param[q1] The quantity, of which q1^P will be returned.
 */
template <int P, typename T, int MASS, int LENGTH, int TIME>
Quantity<T, 1, P * MASS, P * LENGTH, P * TIME>
pow(const Quantity<T, 1, MASS, LENGTH, TIME> &q1) {
	return Quantity<T, 1, P * MASS, P * LENGTH, P * TIME>(std::pow(q1.value(), P));
}

/**
 * Minimum of two scalar quantities.
 */
template <typename T, int MASS, int LENGTH, int TIME>
Quantity<T, 1, MASS, LENGTH, TIME>
minimum(const Quantity<T, 1, MASS, LENGTH, TIME> &q1, const Quantity<T, 1, MASS, LENGTH, TIME> &q2) {
	return (q1 < q2) ? q1 : q2;
}

/**
 * Maximum of two scalar quantities.
 */
template <typename T, int MASS, int LENGTH, int TIME>
Quantity<T, 1, MASS, LENGTH, TIME>
maximum(const Quantity<T, 1, MASS, LENGTH, TIME> &q1, const Quantity<T, 1, MASS, LENGTH, TIME> &q2) {
	return (q1 > q2) ? q1 : q2;
}



// CONSTANTS

// The following common quantity types are predefined for your convenience.

// Scalar quantities:
typedef Quantity<double, 1, 0, 0, 0> Number; //< A scalar dimensionless quantity. Example: Number n = 4;
typedef Quantity<double, 1, 1, 0, 0> Mass; //< A scalar mass. Example: Mass m = 5 * kg;
typedef Quantity<double, 1, 0, 1, 0> Length; //< A scalar length. Example: Length l = 10 * km;
typedef Quantity<double, 1, 0, 0, 1> Time; //< A scalar time. Example: Time t = 5 * days + 10 * hours + 15 * minutes + 10 * seconds;
typedef Quantity<double, 1, 0, 2, 0> Area; //< A scalar area. Example: Area a = 10 * m * 15 * km;
typedef Quantity<double, 1, 0, 3, 0> Volume; //< A scalar volume. Example: Volume v = pow<3>(0.5 * m);
typedef Quantity<double, 1, 0, 1, -1> Velocity; //< A scalar velocity. Example: Velocity v = 100 * km / h;
typedef Quantity<double, 1, 0, 1, -2> Acceleration; //< A scalar acceleration. Example: Acceleration a = 9.81 * m / s / s;
typedef Quantity<double, 1, 1, 1, -2> Force; //< A scalar force. Example: Force f = 1.0 * kg * m / s / s;
typedef Quantity<double, 1, 1, 2, -2> Energy; //< A scalar energy. Example: Energy e = 1.0 * kg * m * m / s / s;

// Two-dimensional quantities:
typedef Quantity<double, 2, 0, 0, 0> Number2D; //< A two-dimensional dimensionless quantity. Example (using C++0x): Number2D n = {1.0, 2.0};
typedef Quantity<double, 2, 0, 1, 0> Length2D; //< A two-dimensional length. Example (using C++0x): Length2D l = {0.0, 1.0};
typedef Quantity<double, 2, 0, 1, -1> Velocity2D; //< A two-dimensional velocity. Example: Velocity2D v; v[0] = 10.0 * m / s;
typedef Quantity<double, 2, 0, 1, -2> Acceleration2D; //< A two-dimensional acceleration.
typedef Quantity<double, 2, 1, 1, -2> Force2D; //< A two-dimensional force.

// Three-dimensional quantities:
typedef Quantity<double, 3, 0, 0, 0> Number3D; //< A three-dimensional dimensionless quantity.
typedef Quantity<double, 3, 0, 1, 0> Length3D; //< A three-dimensional length.
typedef Quantity<double, 3, 0, 1, -1> Velocity3D; //< A three-dimensional velocity.
typedef Quantity<double, 3, 0, 1, -2> Acceleration3D; //< A three-dimensional acceleration.
typedef Quantity<double, 3, 1, 1, -2> Force3D; //< A three-dimensional force.

// The following common units are predefined for your convenience.
// For usage examples, see above.
// Beware of confusing abbreviated units (e.g. m, s) with local variables.

const Mass kilograms(1);
const Mass kilogram(1);
const Mass kg(1);
const Mass grams(1e-3);
const Mass gram(1e-3);
const Mass g(1e-3);

const Length meters(1);
const Length meter(1);
const Length m(1);
const Length kilometers(1e3);
const Length kilometer(1e3);
const Length km(1e3);

const Time seconds(1);
const Time second(1);
const Time s(1);
const Time minutes(60);
const Time minute(60);
const Time min(60);
const Time hours(60 * 60);
const Time hour(60 * 60);
const Time h(60 * 60);
const Time days(24 * 60 * 60);
const Time day(24 * 60 * 60);
const Time d(24 * 60 * 60);

// The following physical constants are predefined for your convenience.

const Quantity<double, 1, -1, 3, -2> G(6.67428e-11); //< Gravitational constant.



#if 0
{ // avoids autoindent of namespace
#endif
}

#endif

