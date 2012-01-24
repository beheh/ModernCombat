/*-- Array-Zusatzfunktionen --*/

//Erweitert Arrays um zusätzliche Funktionen.


#strict 2

/* Zufalls-Array wiedergeben */

global func RandomIndex4K(array ids)
{
  return(ids[Random(GetLength(ids))]);
}

/* Findet das erste Vorlommen von v im Array a und gibt die Position zurück */
//Wenn nichts gefinden wurde wird -1 zurückgegeben.

global func FindInArray4K(array a, v)
{
  return GetIndexOf(v, a);
}

/* Entfernen leerer Einträge */

global func CleanArray4K(array &a)
{
  for(var i = GetLength(a)-1; i >= 0; i--)
    if(!a[i])
      DelArrayItem4K(a,i);
}

/* Entfernung eines bestimmten Eintrags innerhalb eines Arrays */

global func DelArrayItem4K(array &a, int i)
{
  a[i] = 0;

  for(var j = i+1; j < GetLength(a);j++)
  {
    a[j-1] = a[j];
  }

  SetLength (a,GetLength(a)-1);
}

/* Fügt einem Item ein Array hinzu */

global func AddArrayItem4K(array &a, int i, v)
{
  SetLength (a,GetLength(a)+1);

  for(var j = GetLength(a)-2; i > j;j--)
  {
    a[j+1] = a[j];
  }

  a[i] = v;
}

global func AddArray4K(array &aSource, array &aDestination)
{
  var s = GetLength(aSource);
  var d = GetLength(aDestination);
  SetLength(aDestination,d+s); 

  for(var j = 0; j < s; j++)
    aDestination[d+j] = aSource[j];
}