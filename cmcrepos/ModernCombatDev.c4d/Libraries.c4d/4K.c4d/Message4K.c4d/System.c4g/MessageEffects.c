/*-- toll fadende und bunte Nachrichten --*/

#strict

global func Message4K(pObject,szText,iFrames,dwRGBa_1,dwRGBa_2)
{
  AddEffect ("IntShowMessage4K",pObject,1,1,pObject,0,szText,dwRGBa_1,dwRGBa_2,iFrames); 
}

global func Message4K2(pObject,szText,iFrames,dwRGBa,iFadeIn,iFadeOut)
{
  if(!pObject) pObject = this();

  var pObject2 = pObject;

  if(!pObject)
  {
    pObject = CreateObject(ROCK,100,100);//falls global
  }

  var dwRGBa_fade = SetRGBaValue (dwRGBa,0,0);

  Message4K(pObject,szText,iFadeIn,dwRGBa_fade,dwRGBa);//einfaden
  Schedule (Format("Message4K(%d,\"%s\",%d,%d,%d)",pObject2,szText,iFrames,dwRGBa,dwRGBa),iFadeIn,0,pObject);//statischer Teil
  Schedule (Format("Message4K(%d,\"%s\",%d,%d,%d)",pObject2,szText,iFadeOut,dwRGBa,dwRGBa_fade),iFadeIn+iFrames,0,pObject);//ausfaden
}


/* Der Effekt. */
/*
0: Text
1: von RGBa...
2: zu RGBa faden
3: Frames
*/

global func FxIntShowMessage4KStart(object pTarget, int iEffectNumber, int iTemp,szText,dwRGBa_1,dwRGBa_2,iFrames)
{
  EffectVar (0,pTarget,iEffectNumber) = szText;
  EffectVar (1,pTarget,iEffectNumber) = dwRGBa_1;
  EffectVar (2,pTarget,iEffectNumber) = dwRGBa_2;
  EffectVar (3,pTarget,iEffectNumber) = iFrames;
  return(1);
}

global func FxIntShowMessage4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{

  if(iEffectTime > EffectVar (3,pTarget,iEffectNumber))
    return(-1);

  var iFrames  = EffectVar (3,pTarget,iEffectNumber);

  var rgba;

    var dwRGBa_1 = EffectVar (1,pTarget,iEffectNumber);
    var dwRGBa_2 = EffectVar (2,pTarget,iEffectNumber);
    rgba = InterpolateRGBa(dwRGBa_1,dwRGBa_2,0,iFrames,iEffectTime);

  Message ("<c %x>%s</c>",pTarget,rgba,EffectVar (0,pTarget,iEffectNumber));

  return(0);
}

global func FxIntShowMessage4KStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  Message ("",pTarget);
}

global func FxIntShowMessage4KEffect (string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  return(-2);
}

global func FxIntShowMessage4KAdd (object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer,szText,dwRGBa_1,dwRGBa_2,iFrames)
{
  EffectVar (0,pTarget,iEffectNumber) = szText;
  EffectVar (1,pTarget,iEffectNumber) = dwRGBa_1;
  EffectVar (2,pTarget,iEffectNumber) = dwRGBa_2;
  EffectVar (3,pTarget,iEffectNumber) = iFrames;
}




//Interpolations-Funktionen

global func Interpolate(y1,y2,x1,x2,x3)
{
  return(y1 + ((y2 - y1)/(x2 - x1)) * (x3 - x1));
}

global func InterpolateRGBa(RGBa_1,RGBa_2,x1,x2,x3)
{
  var r = Interpolate(GetRGBaValue(RGBa_1,1),GetRGBaValue (RGBa_2,1),x1,x2,x3);
  var g = Interpolate(GetRGBaValue(RGBa_1,2),GetRGBaValue (RGBa_2,2),x1,x2,x3);
  var b = Interpolate(GetRGBaValue(RGBa_1,3),GetRGBaValue (RGBa_2,3),x1,x2,x3);
  var a = Interpolate(GetRGBaValue(RGBa_1,0),GetRGBaValue (RGBa_2,0),x1,x2,x3);

  return(RGBa(r,g,b,a));
}