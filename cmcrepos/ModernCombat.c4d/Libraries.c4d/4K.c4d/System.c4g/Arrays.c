#strict

//Gibt einen zuf�lligen Eintrag eines Arrays zur�ck.
global func RandomIndex4K(array ids)
{
  return(ids[Random(GetLength(ids))]);//Wie billig! Rausnehmen?
}

//Findet das erste Vorlommen von v im Array a und gibt die Position zur�ck.
//Wenn nichts gefinden wurde wird -1 zur�ckgegeben.
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

//L�scht alle leeren Eintr�ge aus einem Array.
global func CleanArray4K(array &a)
{
  for(var i = GetLength(a)-1; i >= 0; i--)
    if(!a[i])
      DelArrayItem4K(a,i);
}

//L�scht ein Item aus einem Array.
global func DelArrayItem4K(array &a, int i)
{
  a[i] = 0;
  
  for(var j = i+1; j < GetLength(a);j++)
  {
    a[j-1] = a[j];
  }
  
  SetLength (a,GetLength(a)-1);
}

//F�gt ein Item einem Array hinzu.
global func AddArrayItem4K(array &a, int i, v)
{
  SetLength (a,GetLength(a)+1);
  
  for(var j = GetLength(a)-2; i > j;j--)
  {
    a[j+1] = a[j];
  }
  
  a[i] = v;
}