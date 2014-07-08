/*-- Spur --*/

#strict 2

local fRemove, iSpeed, pShot, w, l, r, x, y,dist;

func NoWarp()	{return true;}


/* Erstellung */

public func SetFB(int iWidth, int iLength, object pSht, int iDist)
{
  //pShot = pSht;

  w = 1000*iWidth/20;
  l = 1000*iLength/100;
  //Log("%d = 1000*%d/20",w,iWidth);

  var iXDir = GetXDir(pSht,100);
  var iYDir = GetYDir(pSht,100);

  iSpeed = Sqrt(iXDir*iXDir/100+iYDir*iYDir/100);

  if(iDist)
  {
    SetXDir(iXDir,0,100);
    SetYDir(iYDir,0,100);
    dist = iDist;
  }

  r = Angle(0,0,iXDir,iYDir);
  x = GetX(pSht);
  y = GetY(pSht);

  SetAction("Travel");
}

public func Set(int iWidth, int iLength, object pSht)
{
  pShot = pSht;  

  w = 1000*iWidth/20;
  l = 1000*iLength/100;

  var iXDir = GetXDir(pShot,100);
  var iYDir = GetYDir(pShot,100);

  iSpeed = Sqrt(iXDir*iXDir/100+iYDir*iYDir/100);

  SetAction("Travel");
  SetXDir(iXDir,0,100);
  SetYDir(iYDir,0,100);

  r = Angle(0,0,iXDir,iYDir);
  x = GetX();
  y = GetY();

  // richtig positionieren
  SetPosition(GetX(pShot),GetY(pShot));
  Traveling();
}

/* Timer */

private func Traveling()
{
  //Mit Projektil verschwinden
  if(!fRemove)
  {
    if(dist)
    {
      //if(GetActTime() >= 10*dist/iSpeed)
      if(Distance(x,y,GetX(),GetY()) >= dist)
        Remove();
    }
    else
    {
      if(!pShot)
        Remove();
    }
  }

  //Darstellen
  DrawTransform();
  if(pShot) 
    if(pShot->~TrailColor())
      SetClrModulation(pShot->~TrailColor(GetActTime()));

  if(!l) return RemoveObject();
}

public func Remove()
{
  SetXDir();
  SetYDir();
  l = Min(l,10*Distance(x,y,GetX(),GetY()));
  fRemove = true;
}

public func DrawTransform()
{
  if(!fRemove && l < 10*Distance(x,y,GetX(),GetY())) return;

  if(fRemove) l = Max(0,l-iSpeed); 

  var h = Min(l,10*Distance(x,y,GetX(),GetY()));

  var fsin = -Sin(r, 1000), fcos = Cos(r, 1000);

  var xoff = -(GetActMapVal("Facet",GetAction(),0,2)*w/1000)/2-1;
  var yoff = 0;

  var width = +fcos*w/1000, height = +fcos*h/1000;
  var xskew = +fsin*h/1000, yskew = -fsin*w/1000;

  var xadjust = +fcos*xoff + fsin*yoff;
  var yadjust = -fsin*xoff + fcos*yoff;

  SetObjDrawTransform
  (
    width, xskew, xadjust,
    yskew, height, yadjust
  );
}