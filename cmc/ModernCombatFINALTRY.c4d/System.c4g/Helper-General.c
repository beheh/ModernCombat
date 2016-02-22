/*-- Allgemeine Funktionen --*/

//Allgemeine Funktionserweiterungen.

#strict 2


//Im Gegensatz zu PathFree kann hier durch iPrec die Abtast-Distanz angegeben werden.
global func PathFreeAngle4K(int iX, int iY, int iAngle, int iDistance, int iPrec, int iAnglePrec)
{
  if(!iPrec)
    iPrec = 1;

  if(!iAnglePrec)
    iAnglePrec = 1;

  if(this())
  {
    iX -= GetX();
    iY -= GetY();
  }

  var mx = +Sin(iAngle,iDistance),
      my = -Cos(iAngle,iDistance),
      ml = 0;

  while(ml+=iPrec)
  {
    if(GBackSolid(mx*ml/iDistance,my*ml/iDistance))
      return(false);
    if(ml>=iDistance)
      break;
  }
  return(true);
}


//Funktioniert wie PathFreeAngle4K. Gibt Distanz zurück die vom Startpunkt aus noch frei ist.
global func PathFreeAngleEx4K(int iX, int iY, int iAngle, int iDistance, int iPrec, int iAnglePrec)
{
  if(!iPrec)
    iPrec = 1;

  if(!iAnglePrec)
    iAnglePrec = 1;

  var s = iDistance;
  var i = 0;

  for(;i<iDistance;i+=iPrec)
  {
    var x = -Cos(iAngle,i,iAnglePrec);
    var y = -Sin(iAngle,i,iAnglePrec);

    if(s != iDistance)
      s--;
    else
      if(GBackSolid(x,y))
        s--;
  }
  return(iDistance-s);
}

global func PathFreeEx4K(int iStartX, int iStartY, int iEndX, int iEndY, int iPrec)
{
  var iAngle = Angle (iStartX,iStartY,iEndX,iEndY); 
  var iDistance = Distance (iStartX,iStartY,iEndX,iEndY);

  iAngle += 90;

  return(PathFreeAngleEx4K(iEndX,iEndY,iAngle,iDistance,iPrec,1));//*hack*
}

global func PathFree4K(int iStartX, int iStartY, int iEndX, int iEndY, int iPrec)
{
  if(!iPrec)
    iPrec = 1;

  if(this())
  {
    iStartX -= GetX();
    iEndX -= GetX();
    iStartY -= GetY();
    iEndY -= GetY();
  }

  var mx = iEndX-iStartX,
      my = iEndY-iStartY,
      ml = 0,
      d = Distance(iStartX,iStartY,iEndX,iEndY);

  while(ml+=iPrec)
  {
    if(GBackSolid(iStartX+(mx*ml/d),iStartY+(my*ml/d)))
      return(false);
    if(ml>=d)
      break;
  }
  return(true);
}

global func PathFreeObject4K(object pA, object pB, int iPrec)
{
  if(!pA) return;
  if(!pB) pB = this;
  if(!pB) return;

  return(PathFree4K(GetX(pA),GetY(pA),GetX(pB),GetY(pB),iPrec));
}

//Plaziert ein Objekt. (PlaceInMaterial() fand ich iwie doof. :|)
global func PlaceObject4K(object pObject,int iX,int iY,int iWidth,int iHeight,int iRetries,int bSet2Ground)//Platziert ein Objekt an der Luft. Unter- oder Oberirdisch.
{
  var x = RandomX(iX,iWidth);
  var y = RandomX(iY,iHeight); 

  while(iRetries >= 0)
  {
    if(!GBackSemiSolid (x,y))
      break;

    x = RandomX(iX,iWidth);
    y = RandomX(iY,iHeight); 

    iRetries--;
  }

  SetPosition(x,y,pObject);

  if(bSet2Ground)
  {
    while(!Stuck(pObject))
      SetPosition(GetX(pObject),GetY(pObject)+1,pObject);
  }

  SetPosition(GetX(pObject),GetY(pObject)-1,pObject);

  return(1);
}

//Platziert ein Objekt auf einem Tisch. (Tisch sowie Such-Rechteck sind optional.)
global func PlaceOnTable4K(object pObj, object pTable, int iX, int iY, int iW, int iH)
{
  if(!iX && !iY && !iW && !iH)
  {
    iX = 0;
    iY = 0;
    iW = LandscapeWidth();
    iH = LandscapeHeight();
  }

  if(!pTable)
  {
    var a = FindObjects(Find_InRect(iX,iY,iW,iH),Find_Or(Find_ID(OTBL),Find_ID(LTBL),Find_ID(TABL)));//Ouch! >.<
    pTable = a[Random(GetLength(a)-1)];
  }

  if(Inside(GetR(pTable),-20,20))
  {
    var xoff = Random(GetDefCoreVal ("Width",0,GetID(pTable))/2);
    if(Random(1))
      xoff = -xoff;

    SetPosition (GetX(pTable)+xoff,GetY(pTable)-10,pObj);
    return(true);
  }
  return(false);
}

//Findet mögliche Kontainer und gibt einen Objektarray zurück. (ID-Liste sowie Such-Reckteck sind optional.)
global func FindContainers4K(array aIDList, int iX, int iY, int iW, int iH)
{
  if(!iX && !iY && !iW && !iH)
  {
    iX = 0;
    iY = 0;
    iW = LandscapeWidth();
    iH = LandscapeHeight();
  }

  var a;
  if(!GetLength(aIDList))
  {
    a = FindObjects(Find_InRect(iX,iY,iW,iH),Find_NoContainer(),Find_Category(C4D_Structure|C4D_Vehicle|C4D_Object),Find_OCF(OCF_Container));
  }
  else
  {
    a = CreateArray();
    for(var idobj in aIDList)
      AddArray4K(FindObjects(Find_InRect(iX,iY,iW,iH),Find_ID(idobj)),a);
  }
  return(a);
}

//Errechnet Offset-Koordinaten aus iX/iY.
global func GetOffset4K(int &iX, int &iY, int iPrec, object pObject)
{
  if(!iPrec)
    iPrec = 1;

  if(!pObject)
    pObject = this();

  var d = Distance (iX,iY);
  var r = Angle (iX,iY) + GetR(pObject) + 90;

  iX = Cos(r,d,iPrec);
  iY = Sin(r,d,iPrec);
}

//Wie GetOffset4K nur eben mit iX als return-Wert. (Eigentlich unnütz, raus?)
global func GetXOffset4K(int iX, int iY, int iPrec, object pObject)
{
  if(!iPrec)
    iPrec = 1;

  if(!pObject)
    pObject = this();

  var d = Distance (iX,iY);
  var r = Angle (iX,iY) + GetR(pObject) + 90;

  return(Cos(r,d,iPrec));
}

//Wie GetOffset4K nur eben mit iY als return-Wert. (Eigentlich unnütz, raus?)
global func GetYOffset4K(int iX, int iY, int iPrec, object pObject)
{
  if(!iPrec)
    iPrec = 1;

  if(!pObject)
    pObject = this();

  var d = Distance (iX,iY);
  var r = Angle (iX,iY) + GetR(pObject) + 90;

  return(Sin(r,d,iPrec));
}

//Gibt es das Material X (Achtung MaterialID!) in der Umgebung? (ToDo: Soll Prozentsatz ausgeben.)
global func FindMaterial4K(int iMat, int iDistance, int iX, int iY, int iPrec)
{
  if(!MaterialName(iMat)) return(false);
  if(!iDistance) return(false);
  if(!iPrec) iPrec = 30;

  for(var x = -iDistance; x < iDistance; x += 10)
    for(var y =- iDistance; y < iDistance;y += 10) 
      if(GetMaterial(x,y) == iMat)
        return(true);

  return(false);
}

//Tötet ein Lebewesen wie ein normales Objekt.
global func SilentKill4K(object pObject, bool fEjectContents)
{
/*  Achtung!
  Diese Funktion ist nur mit äußerster Vorsicht zu genießen,
  denn es sollte darauf geachtet werden,
  dass bei Death() usw. manchmal noch wichtige Dinge getan werden müssen!
*/
  if(!pObject) pObject = this();
  if(!pObject) return;

  SetAlive (false, pObject);

  var cat = GetCategory(pObject);
  var cat = SetBit(cat , 3,  false);
  SetCategory (cat, pObject);
  AddEffect("SilentKill", pObject, 1);

  RemoveObject(pObject,fEjectContents);
}

//ObjectDistance gibt es, aber ObjectAngle nicht? :°
global func ObjectAngle(object pA, object pB)
{
  return(Angle(GetX(pA),GetY(pA),GetX(pB),GetY(pA)));
}

//Errechnet die Oberflächennormale.
global func SurfaceNormal4K(int iX, int iY, int iDist, int iPrec)
{
  if(!iDist) iDist = 1;

  var vX,vY;//2D-Vektor

  for(var x = -iDist; x <= +iDist; x++)
  {
    for(var y = -iDist; y <= +iDist; y++)
    {
      if(GBackSolid(x+iX,y+iY))
      {
        vX += x;
        vY += y;
      }
    }
  }
  return(Angle(0,0,vX,vY,iPrec));
}
