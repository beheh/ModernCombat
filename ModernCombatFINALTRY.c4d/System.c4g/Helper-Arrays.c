/*-- Array-Zusatzfunktionen --*/

//Erweitert Arrays um zusätzliche Funktionen.

#strict 2


/* Zufalls-Array wiedergeben */

global func RandomArrayIndex(array ids)
{
  return(ids[Random(GetLength(ids))]);
}

/* Findet das erste Vorlommen von v im Array a und gibt die Position zurück */
//Wenn nichts gefinden wurde wird -1 zurückgegeben.

global func FindInArray(array a, v)
{
  return GetIndexOf(v, a);
}

global func ArrayContains(array a, v)
{
  return GetIndexOf(v, a) + 1;
}

/* Entfernen leerer Einträge */

global func CleanArray(array &a)
{
  for(var i = GetLength(a)-1; i >= 0; i--)
    if(!a[i])
      DelArrayItem(a,i);
}

/* Entfernung eines bestimmten Eintrags innerhalb eines Arrays */

global func DelArrayItem(array &a, int i)
{
  a[i] = 0;

  for(var j = i+1; j < GetLength(a);j++)
  {
    a[j-1] = a[j];
  }

  SetLength (a,GetLength(a)-1);
}

/* Fügt einem Item ein Array hinzu */

/*global func AddArrayItem(array &a, int i, v)
{
  SetLength (a,GetLength(a)+1);

  for(var j = GetLength(a)-2; i > j;j--)
  {
    a[j+1] = a[j];
  }

  a[i] = v;
}*/

global func AddArray(array &aSource, array &aDestination)
{
  var s = GetLength(aSource);
  var d = GetLength(aDestination);
  SetLength(aDestination,d+s); 

  for(var j = 0; j < s; j++)
    aDestination[d+j] = aSource[j];
}

/** Gibt den höchsten / niedrigsten Wert eines Arrays zurück **/

global func GetMinArrayVal(array a, bool fPos, bool fEqual)
{
  if(!fEqual)
    var lowest = a[0];
  else
    var lowest = 0;
  var pos; 
  var fFirst = true;
  for(var i = 0; i < GetLength(a); i++)
  {
    if(fFirst || a[i] < lowest)
    {
      lowest = a[i];
      pos = i;
      fFirst = false;
    }
  }

  if(fPos)
    return pos;

  return lowest;
}

global func GetMaxArrayVal(array a, bool fPos, bool fEqual)
{
  if(!a)
    return -1;

  if(!fEqual)
    var highest = a[0];
  else
    var highest = 0;

  var pos = -1;
  for(var i = 0; i < GetLength(a); i++)
  {
    if(a[i] > highest)
    {
      highest = a[i];
      pos = i;
    }
  }

  if(fPos)
    return pos;

  return highest;
}

/* Alte Syntax für Abwärtskompatibilität */

global func RandomIndex4K(array ids)			{return RandomArrayIndex(ids);}
global func FindInArray4K(array a, v)			{return FindInArray(a, v);}
global func CleanArray4K(array &a)			{return CleanArray(a);}
global func DelArrayItem4K(array &a, int i)		{return DelArrayItem(a, i);}
global func AddArray4K(array &aSrc, array &aDst)	{return AddArray(aSrc, aDst);}//global func AddArrayItem4K(array &a, int i, v)		{return AddArrayItem(a, i, v);}