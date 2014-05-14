#include "artemis/Aspect.h"
#include "artemis/ComponentType.h"
#include <cstdarg>

namespace artemis
{

/*
Aspect * Aspect::all(ComponentType ct1)
{
   allSet.set(ct1);
   return this;
}

Aspect * Aspect::all(ComponentType ct1, ComponentType ct2)
{
   allSet.set(ct1).set(ct2);
   return this;
}

Aspect * Aspect::all(ComponentType ct1, ComponentType ct2, ComponentType ct3)
{
   allSet.set(ct1).set(ct2).set(ct3);
   return this;
}
*/

Aspect * Aspect::exclude(size_t n, ...)
{
   ComponentType val;
   std::va_list vl;
   va_start(vl, n);
   for (size_t i = 0; i < n; ++i)
   {
      val = va_arg(vl,ComponentType);
      exclusionSet.set(val);
   }
   va_end(vl);

   return this;
}

Aspect * Aspect::one(size_t n, ...)
{
   ComponentType val;
   std::va_list vl;
   va_start(vl, n);
   for (size_t i = 0; i < n; ++i)
   {
      val = va_arg(vl,ComponentType);
      oneSet.set(val);
   }
   va_end(vl);

   return this;
}

/*

Aspect * Aspect::getAspectForAll(ComponentType ct1)
{
   Aspect *aspect = new Aspect();
   aspect->all(ct1);
   return aspect;
}

Aspect * Aspect::getAspectForAll(ComponentType ct1, ComponentType ct2)
{
   Aspect *aspect = new Aspect();
   aspect->all(ct1, ct2);
   return aspect;
}

Aspect * Aspect::getAspectForAll(ComponentType ct1, ComponentType ct2, ComponentType ct3)
{
   Aspect *aspect = new Aspect();
   aspect->all(ct1, ct2, ct3);
   return aspect;
}
*/

Aspect * Aspect::getAspectForOne(size_t n, ...)
{
   Aspect *aspect = new Aspect();
   std::va_list vl;
   va_start(vl, n);
   aspect->one(n, vl);
   va_end(vl);
   return aspect;
}

}
