#strict

//Gibt einen zufälligen Eintrag eines Arrays zurück.
global func RandomIndex4K(array ids)
{
  return(ids[Random(GetLength(ids))]);//Wie billig! Rausnehmen?
}

//Findet das erste Vorlommen von v im Array a und gibt die Position zurück.
//Wenn nichts gefinden wurde wird -1 zurückgegeben.
global func FindInArray4K(array a, v)
{
  for(var i = GetLength(a); i >= 0; i--)
  {
    if(a[i] == v)
    {
      return(i);
    }
  }
  return(-1);
}

//Löscht alle leeren Einträge aus einem Array.
global func CleanArray4K(array &a)
{
  for(var i = GetLength(a)-1; i >= 0; i--)
    if(!a[i])
      DelArrayItem4K(a,i);
}

//Löscht ein Item aus einem Array.
global func DelArrayItem4K(array &a, int i)
{
  a[i] = 0;
  
  for(var j = i+1; j < GetLength(a);j++)
  {
    a[j-1] = a[j];
  }
  
  SetLength (a,GetLength(a)-1);
}

//Fügt ein Item einem Array hinzu.
global func AddArrayItem4K(array &a, int i, v)
{
  SetLength (a,GetLength(a)+1);
  
  for(var j = GetLength(a)-2; i > j;j--)
  {
    a[j+1] = a[j];
  }
  
  a[i] = v;
}