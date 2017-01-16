﻿// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Vec.h"
#include "Matrix.h"
#include "../Dimensions/RadiansAndDegrees.h"

namespace GX_STL
{
namespace GXMath
{
	
#	define _QUAT_OPERATOR1( _op_, _right_, _func_ ) \
		Self &	operator _op_##= (const _right_& right)			_func_ \
		Self	operator _op_    (const _right_& right) const	{ return Self(*this) _op_##= right; }

#	define _QUAT_OPERATOR2( _op_ ) \
		_QUAT_OPERATOR1( _op_, T, { x _op_##= right;  y _op_##= right;  z _op_##= right;  w _op_##= right;  return *this; } )

#	define _QUAT_OPERATOR3( _op_ ) \
		_QUAT_OPERATOR1( _op_, Self, { x _op_##= right.x;  y _op_##= right.y;  z _op_##= right.z;  w _op_##= right.w;  return *this; } )

#	define _QUAT_OPERATOR( _op_ ) \
		_QUAT_OPERATOR2( _op_ ); \
		_QUAT_OPERATOR3( _op_ )



	//
	// Quaternion
	//

	template <typename T>
	struct Quaternion : public CompileTime::CopyQualifiers< T >
	{
		// types
		typedef Quaternion<T>		Self;
		typedef T					value_t;
		typedef Vec<T,3>			vec3_t;
		typedef Matrix<T,3,3>		mat3_t;
		typedef Matrix<T,4,4>		mat4_t;
		typedef bool				_is_quaternion;


		// variables
		T	x, y, z, w;
		

		// constructors
		Quaternion (GX_DEFCTOR): x(0), y(0), z(0), w(1) {}
		Quaternion (T X, T Y, T Z, T W): x(X), y(Y), z(Z), w(W) {}
		Quaternion (T X, T Y, T Z);
		Quaternion (const vec3_t &vec);
		Quaternion (const mat3_t &matrix);
		Quaternion (const mat4_t &matrix);
		
		// unary operators
		Self		operator - () const;
		bool		operator ! () const;

		// binary operators
		T		&	operator [] (usize index);
		const T &	operator [] (usize index) const;

		bool		operator == (const Self &right)	const;
		bool		operator != (const Self &right)	const;

		Self &		operator =  (const Self &right);
		Self &		operator =  (const mat4_t &right);
		Self &		operator =  (const mat3_t &right);
		Self &		operator =  (const vec3_t &right);

		Self		operator *  (const Self &right)	const;
		vec3_t		operator *  (const vec3_t &right) const;
		Self &		operator *= (const Self &right);
		
		_QUAT_OPERATOR( +  );
		_QUAT_OPERATOR( -  );
		_QUAT_OPERATOR2( * );
		_QUAT_OPERATOR( /  );
		_QUAT_OPERATOR( %  );
		_QUAT_OPERATOR( &  );
		_QUAT_OPERATOR( |  );
		_QUAT_OPERATOR( ^  );
		_QUAT_OPERATOR( >> );
		_QUAT_OPERATOR( << );

		
		// methods
		T		*	 ptr ()							{ return PointerCast< T >( this ); }
		const T	*	 ptr () const					{ return PointerCast< T >( this ); }

		static usize Count ()						{ return 4; }

		Self &	Set (T X, T Y, T Z);
		Self &	Set (T X, T Y, T Z, T W);
		Self &	Set (const vec3_t &point)			{ return ( *this = point ); }
		Self &	Set (const mat3_t &matrix)			{ return ( *this = matrix ); }
		Self &	Set (const mat4_t &matrix)			{ return ( *this = matrix ); }
		
		T		Dot (const Self &right) const;

		bool	IsZero () const;

		Self	Normalized () const;
		Self &	Normalize ();
		
		vec3_t	GetDirection () const;

		Self &	Inverse ();
		Self	Inversed () const;

		Self &	Slerp (Self left, Self right, T interpolation);

		Self &	FromAngleAxis (Radians<T> angle, const vec3_t &axis);

		void	ToAngleAxis (OUT Radians<T> &angle, OUT vec3_t &axis) const;
		void	ToEuler (OUT Vec<Radians<T>,3> &euler) const;

		//Self & FromSpherical (const Vec<T,2> &sAngles);
		//void	ToSpherical (Vec<T,2> &sAngles) const;

		Self &	MakeIdentity ();
		Self &	RotationFromTo (const vec3_t &fromDir, const vec3_t &toDir);

		Self &	RotationX (Radians<T> angle);
		Self &	RotationY (Radians<T> angle);
		Self &	RotationZ (Radians<T> angle);
		Self &	Rotation (const Vec<Radians<T>,3> &angle);

		template <typename T2>
		const Quaternion<T2> Convert () const;

		template <typename B>
		const B	To () const;

		Vec<T,4> ToVec () const		{ return Vec<T,4>( x, y, z, w ); }

		String	ToString ()		const;
		String	ToStringFull ()	const;
	};

	

//--------------------------- Math Func ---------------------------//
	
	template <typename T>
	inline bool IsZero (const Quaternion<T>& val)
	{
		return val.IsZero();
	}



//----------------------------- Quaternion -----------------------//

	template <typename T>
	inline Quaternion<T>::Quaternion (T X, T Y, T Z): x(0), y(0), z(0), w(1)
	{
		Set( X, Y, Z );
	}


	template <typename T>
	inline Quaternion<T>::Quaternion (const vec3_t &vec): x(0), y(0), z(0), w(1)
	{
		Set( vec.x, vec.y, vec.z );
	}


	template <typename T>
	inline Quaternion<T>::Quaternion (const mat3_t &matrix): x(0), y(0), z(0), w(1)
	{
		Set( matrix );
	}


	template <typename T>
	inline Quaternion<T>::Quaternion (const mat4_t &matrix): x(0), y(0), z(0), w(1)
	{
		Set( matrix );
	}
	
		
	template <typename T>
	inline T & Quaternion<T>::operator [] (usize index)
	{
		ASSUME( index < 4 );
		return ptr()[index];
	}

		
	template <typename T>
	inline const T & Quaternion<T>::operator [] (usize index) const
	{
		ASSUME( index < 4 );
		return ptr()[index];
	}


	template <typename T>
	inline bool Quaternion<T>::operator == (const Self &right) const
	{
		return ( Equals( x, right.x ) and Equals( y, right.y ) and
				 Equals( z, right.z ) and Equals( w, right.w ) );
	}
	

	template <typename T>
	inline bool Quaternion<T>::operator != (const Self &right) const
	{
		return not ( *this == right );
	}
	

	template <typename T>
	inline Quaternion<T> & Quaternion<T>::operator = (const vec3_t &right)
	{
		Set( right.x, right.y, right.z );
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::operator = (const Self &right)
	{
		x = right.x;
		y = right.y;
		z = right.z;
		w = right.w;
		return *this;
	}
	

	template <typename T>
	inline Quaternion<T> & Quaternion<T>::operator = (const mat3_t &right)
	{
		T			s		= T(0);
		const T		trace	= right(0,0) + right(1,1) + right(2,2);
		Self &		q		= *this;

		if ( trace > T(0) )
		{
			s = Sqrt( trace + T(1) );
			w = s * T(0.5);
			s = T(0.5) / s;

			x = ( right(1,2) - right(2,1) ) * s;
			y = ( right(2,0) - right(0,2) ) * s;
			z = ( right(0,1) - right(1,0) ) * s;
		}
		else
		{
			const int	nxt[3] = {1,2,0};
			int			i = 0,
						j = 0,
						k = 0;

			if ( right(1,1) > right(0,0) )
				i = 1;

			if ( right(2,2) > right(1,1) )
				i = 2;

			j = nxt[i];
			k = nxt[j];
			s = Sqrt( ( right(i,i) - ( right(j,j) + right(k,k) ) ) + T(1) );

			q[i] = s * T(0.5);
			s = T(0.5) / s;

			q[3] = ( right(j,k) - right(k,j) ) * s;
			q[j] = ( right(i,j) + right(j,i) ) * s;
			q[k] = ( right(i,k) + right(k,i) ) * s;
		}
		return Normalize();
	}
	

	template <typename T>
	inline Quaternion<T> & Quaternion<T>::operator = (const mat4_t &right)
	{
		T			s		= T(0);
		const T		trace	= right(0,0) + right(1,1) + right(2,2);
		Self &		q		= *this;

		if ( trace > T(0) )
		{
			s = Sqrt( trace + T(1) );
			w = s * T(0.5);
			s = T(0.5) / s;

			x = ( right(1,2) - right(2,1) ) * s;
			y = ( right(2,0) - right(0,2) ) * s;
			z = ( right(0,1) - right(1,0) ) * s;
		}
		else
		{
			const int	nxt[3] = { 1, 2, 0 };
			int			i = 0,
						j = 0,
						k = 0;

			if ( right(1,1) > right(0,0) )
				i = 1;

			if ( right(2,2) > right(1,1) )
				i = 2;

			j = nxt[i];
			k = nxt[j];
			s = Sqrt( ( right(i,i) - ( right(j,j) + right(k,k) ) ) + T(1) );

			q[i] = s * T(0.5);
			s = T(0.5) / s;

			q[3] = ( right(j,k) - right(k,j) ) * s;
			q[j] = ( right(i,j) + right(j,i) ) * s;
			q[k] = ( right(i,k) + right(k,i) ) * s;
		}
		return Normalize();
	}
	

	template <typename T>
	inline Quaternion<T> Quaternion<T>::operator - () const
	{
		return Inversed();
	}
	
		
	template <typename T>
	inline bool Quaternion<T>::operator ! () const
	{
		return not x and not y and not z and not w; 
	}


	template <typename T>
	inline Quaternion<T> Quaternion<T>::operator * (const Self &right) const
	{
		Self	temp;

		temp.w = (right.w * w) - (right.x * x) - (right.y * y) - (right.z * z);
		temp.x = (right.w * x) + (right.x * w) + (right.y * z) - (right.z * y);
		temp.y = (right.w * y) + (right.y * w) + (right.z * x) - (right.x * z);
		temp.z = (right.w * z) + (right.z * w) + (right.x * y) - (right.y * x);

		return temp;
	}
	

	template <typename T>
	inline Vec<T,3> Quaternion<T>::operator * (const vec3_t &right) const
	{
		vec3_t const	qvec(x, y, z);
		vec3_t const	uv  = Cross( qvec, right );
		vec3_t const	uuv = Cross( qvec, uv );

		return right + ((uv * w) + uuv) * T(2);
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::operator *= (const Self &right)
	{
		return ( *this = (*this) * right );
	}


	template <typename T>
	inline T Quaternion<T>::Dot (const Self &right) const
	{
		return (x * right.x) + (y * right.y) + (z * right.z) + (w * right.w);
	}
	
	
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Set (T X, T Y, T Z)
	{
		T	sr, cr,
			sp, cp,
			sy, cy;

		SinCos( X * T(0.5), sr, cr );
		SinCos( Y * T(0.5), sp, cp );
		SinCos( Z * T(0.5), sy, cy );

		const T	cpcy = cp * cy;
		const T	spcy = sp * cy;
		const T	cpsy = cp * sy;
		const T	spsy = sp * sy;

		x = sr * cpcy - cr * spsy;
		y = cr * spcy + sr * cpsy;
		z = cr * cpsy - sr * spcy;
		w = cr * cpcy + sr * spsy;

		return Normalize();
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Set (T X, T Y, T Z, T W)
	{
		z = X;
		y = Y;
		z = Z;
		w = W;
		return *this;
	}
	

	template <typename T>
	inline bool Quaternion<T>::IsZero () const
	{
		return ( GXMath::IsZero(x) and GXMath::IsZero(y) and GXMath::IsZero(z) and GXMath::Equals( w, T(1) ) );
	}


	template <typename T>
	inline Quaternion<T> Quaternion<T>::Normalized () const
	{
		const T	n = x*x + y*y + z*z + w*w;

		if ( n == T(1) )
			return *this;

		return ( (*this) * ( T(1) / Sqrt(n) ) );
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Normalize ()
	{
		const T	n = x*x + y*y + z*z + w*w;

		if ( n == T(1) )
			return *this;

		return ( *this *= T(1) / Sqrt(n) );
	}
	
		
	template <typename T>
	inline Vec<T,3> Quaternion<T>::GetDirection () const
	{
		return vec3_t(	T(2) * x * z + T(2) * y * w,
						T(2) * z * y - T(2) * x * w,
						T(1) - T(2) * x * x - T(2) * y * y );
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Inverse ()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	
	template <typename T>
	inline Quaternion<T> Quaternion<T>::Inversed () const
	{
		return Self( *this ).Inverse();
	}

	
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Slerp (Self left, Self right, T interpolation)
	{
		T	angle = left.Dot( right );

		if ( angle < T(0) )
		{
			left	*= T(-1);
			angle	*= T(-1);
		}

		T	scale,
			invscale;

		if ( angle + T(1) > T(0.05) )
		{
			if ( T(1) - angle >= T(0.05) )
			{
				const T	theta		= ACos( angle );
				const T	invsintheta	= T(1) / ( Sin( theta ) );
				scale				= Sin( theta * ( T(1) - interpolation ) ) * invsintheta;
				invscale			= Sin( theta * interpolation ) * invsintheta;
			}
			else
			{
				scale	 = T(1) - interpolation;
				invscale = interpolation;
			}
		}
		else
		{
			right.Set( -left.y, left.x, -left.w, left.z );

			scale	 = Sin( Pi<T> * ( T(0.5) - interpolation ) );
			invscale = Sin( Pi<T> * interpolation );
		}

		return ( *this = ( left * scale ) + ( right * invscale ) );
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::FromAngleAxis (Radians<T> angle, const vec3_t &axis)
	{
		const T	half_angle	= T(0.5) * angle;
		const T	sin			= Sin( half_angle );

		x = sin * axis.x;
		y = sin * axis.y;
		z = sin * axis.z;
		w = Cos( half_angle );

		return *this;
	}
	

	template <typename T>
	inline void Quaternion<T>::ToAngleAxis (OUT Radians<T> &angle, OUT vec3_t &axis) const
	{
		const T scale = Sqrt( x*x + y*y + z*z );

		if ( GXMath::IsZero( scale ) or w > T(1) or w < T(-1) )
		{
			angle  = T(0);
			axis.x = T(0);
			axis.y = T(1);
			axis.z = T(0);
		}
		else
		{
			const T invscale = T(1) / scale;

			angle  = T(2) * ACos( w );
			axis.x = x * invscale;
			axis.y = y * invscale;
			axis.z = z * invscale;
		}
	}
	

	template <typename T>
	inline void Quaternion<T>::ToEuler (OUT Vec<Radians<T>,3> &euler) const
	{
		const T	sqw = w*w;
		const T	sqx = x*x;
		const T	sqy = y*y;
		const T	sqz = z*z;

		euler.x = Radians<T>( ATan( T(2) * (y*z +x*w), -sqx - sqy + sqz + sqw ) );
		euler.y = Radians<T>( ASin( Clamp( T(-2) * (x*z - y*w), T(-1), T(1) ) ) );
		euler.z = Radians<T>( ATan( T(2) * (x*y +z*w), sqx - sqy - sqz + sqw ) );
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::MakeIdentity ()
	{
		w = T(1);
		x = T(0);
		y = T(0);
		z = T(0);
		return *this;
	}
	

	template <typename T>
	inline Quaternion<T> & Quaternion<T>::RotationFromTo (const vec3_t &fromDir, const vec3_t &toDir)
	{
		const vec3_t	v0 = fromDir.Normalized();
		const vec3_t	v1 = toDir.Normalized();
		const T			d  = Dot( v0, v1 );

		if ( d >= T(1) )
		{
			return MakeIdentity();
		}
		else
		if ( d <= T(-1) )
		{
			vec3_t	axis( T(1), T(0), T(0) );

			axis = Cross( axis, vec3_t( x, y, z ) );

			if ( IsZero( axis.Length() ) )
			{
				axis.Set( T(0), T(1), T(0) );
				axis.Cross( vec3_t( x, y, z ) );
			}
			return FromAngleAxis( Pi<T>, axis );
		}

		const T			s	 = Sqrt( (1 + d) * T(2) );
		const T			invs = T(1) / s;
		const vec3_t	c	 = v0.Cross(v1) * invs;
		
		x = c.x;
		y = c.y;
		z = c.z;
		w = s * T(0.5);

		return *this;
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::RotationX (Radians<T> angle)
	{
		const T	half_angle = angle * T(0.5);

		x = Sin( half_angle );
		y = T(0);
		z = T(0);
		w = Cos( half_angle );

		return *this;
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::RotationY (Radians<T> angle)
	{
		const T	half_angle = angle * T(0.5);

		x = T(0);
		y = Sin( half_angle );
		z = T(0);
		w = Cos( half_angle );

		return *this;
	}


	template <typename T>
	inline Quaternion<T> & Quaternion<T>::RotationZ (Radians<T> angle)
	{
		const T	half_angle = angle * T(0.5);

		x = T(0);
		y = T(0);
		z = Sin( half_angle );
		w = Cos( half_angle );

		return *this;
	}
	

	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Rotation (const Vec<Radians<T>,3> &angle)
	{
		*this = Self().RotationX( angle.x ) * Self().RotationY( angle.y ) * Self().RotationZ( angle.z );
		return *this;
	}

	
	template <typename T>
	template <typename T2>
	inline const Quaternion<T2>  Quaternion<T>::Convert () const
	{
		return Quaternion<T2>( (T2)x, (T2)y, (T2)z, (T2)w );
	}
	

	template <typename T>
	template <typename B>
	inline const B  Quaternion<T>::To () const
	{
		STATIC_ASSERT( B::_is_quaternion(true), "type is not quaternion" );

		return Convert< B::value_t >();
	}


	template <typename T>
	inline String Quaternion<T>::ToString () const
	{
		return ( String() << x << ", " << y << ", " << z << ", " << w );
	}

	
	template <typename T>
	inline String Quaternion<T>::ToStringFull () const
	{
		return ( String("x: ") << x << ", y: " << y << ", z: " << z << ", w: " << w );
	}


#	undef _QUAT_OPERATOR
#	undef _QUAT_OPERATOR1
#	undef _QUAT_OPERATOR2
#	undef _QUAT_OPERATOR3


}	// GXMath

namespace GXTypes
{
	
	template <typename T>
	struct Hash< GXMath::Quaternion<T> > :
		private Hash< GXMath::Vec<T,4> >
	{
		typedef GXMath::Quaternion<T>		key_t;
		typedef Hash< GXMath::Vec<T,4> >	base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.ToVec() );
		}
	};

}	// GXTypes
}	// GX_STL
