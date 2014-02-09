/* Globale Partikeleffekte */

//Stellt global nutzbare Effekte zur Verfügung.

#strict 2


/* Mündungsfeuer */

global func MuzzleFlash(int iSize, object pClonk, int iX, int iY, int iAngle, int iColor, int type)
{
  //Ziel lokalisieren
  if(!pClonk) pClonk = this;

  //Mündungsfeuer erstellen
  if(!type) 
    type = 3;
  CreateParticle(Format("MuzzleFlash%d", type),iX,iY,
  		+Sin(iAngle,500) - GetXDir(pClonk),
  		-Cos(iAngle,500) - GetYDir(pClonk),
  		iSize*5,iColor,pClonk);

  //Nichts weiter bei keiner Dunkelheit unternehmen
  if(!IsDark())
    return;

  //Farbe für Licht setzen sofern nicht gegeben
  if(!iColor)
    iColor = RGB(255,255,255);
  var r,g,b,a;
  SplitRGBaValue(iColor,r,g,b,a);
  iColor = RGBa(r,g,b,Min(a+65,255));

  //Lichtkegel erzeugen
  AddLightFlash(iSize*25, iX, iY, iColor);
}

/* Splash */

global func Splash(int tx, int ty, int amount)
{
  //Nur, wenn darüber frei ist
  if(GBackSemiSolid(tx, ty-15)) return;

  var mat = GetMaterial(tx, ty);

  //Prüfen ob flüssig
  if(Inside(GetMaterialVal("Density", "Material", mat), 25, 49))
  {
    var x = tx, y = ty, xdir, ydir = -10, g = GetGravity();
    SetGravity(0);

    var fSurface = SimFlight(x, y, xdir, ydir, -1, 24, 50);
    SetGravity(g);

    if(!fSurface)
      return false;
    else
      y -= 2;

    for(var cnt = 0; cnt < amount; cnt++)
      if(GBackLiquid(tx, ty) && !GBackSemiSolid(tx, y))
        InsertMaterial(ExtractLiquid(tx, ty), tx, y, Random(15)-7, -Random(20));
  }

  return true;
}

/* Rauch */

//Überprüft auf Wasser und erstellt in gleichem Luftblasen statt Rauch.

global func CastSmoke(string szParticle, int iAmount, int iLevel, int iX, int iY, int iSize1, int iSize2, int dwColor, int dwColor2)
{
  if(!dwColor)
    dwColor = RGBa(255,255,255,120);
  if(!dwColor2)
    dwColor2 = dwColor;

  if(!GBackLiquid(iX,iY))
    CastParticles(szParticle, iAmount, iLevel, iX, iY, iSize1, iSize2, dwColor, dwColor2);
  else
    CastObjects(FXU1,iAmount*2/3,25,iX,iY);
}

global func SmokeBurst(int iSize, int iX, int iY, int iAngle, object pAttach, int dwColor)
{
  if(GetEffectData(EFSM_BulletEffects) < 1) return;

  if(!pAttach)
  {
    if(!ObjectCount(BOOM)) pAttach = CreateObject(BOOM,0,0,-1);
    else pAttach = FindObject(BOOM);
  }

  if(!dwColor)
  {
    dwColor = RGB(255,255,255);
  }

  var mx = +Sin(iAngle,6),
	my = -Cos(iAngle,6);

  CreateParticle("GunSmoke",iX,iY,0,0,
		iSize*5,dwColor,pAttach,1);

  CreateParticle("GunSmoke",iX,iY,mx/2,my/2,
		iSize*4,SetRGBaValue(dwColor,64,0),pAttach,1);

  CreateParticle("GunSmoke",iX,iY,mx,my,
		iSize*3,SetRGBaValue(dwColor,128,0),pAttach,1);
}

/* Blut */

static const SplatterScale = 50;

global func BloodBurst(int iSize, int iX, int iY, int iColor)
{
  if(!iColor)
    iColor = RGB(80+Random(105));
  CreateParticle("BloodBurst",iX,iY,0,1,iSize*5,iColor);
}

global func BloodSplatter(int iSize, int iX, int iY, int iColor)
{
  if(!GetEffectData(EFSM_Blood)) return;

  //Nicht in der Luft
  if(GetMaterialVal("Density","Material",GetMaterial(iX,iY)) != 0
  || GetMaterial(iX,iY) == -1) return;

  var boom;
  if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,0,0,-1);
  else boom = FindObject(BOOM);

  if(!iColor)
    iColor = RGB(80+Random(105));
  var r = Random(360);
  var xdir = Sin(r,100);
  var ydir = -Cos(r,100);
  CreateParticle("BloodSplatter",iX,iY,xdir,ydir,iSize*5,iColor,boom,1);
}

global func BloodSplatter2(int iSize, int iX, int iY, int iAngle, int iColor)
{
  if(!GetEffectData(EFSM_Blood)) return;

  if(GetMaterialVal("Density","Material",GetMaterial(iX,iY)) != 0
  || GetMaterial(iX,iY) == -1) return;

  iAngle += 180;
  if(!iColor)
    iColor = RGB(80+Random(105));

  var boom;
  if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,0,0,-1);
  else boom = FindObject(BOOM);

  CreateParticle("BloodSplatter2",iX+Sin(iAngle,iSize/2-5),iY-Cos(iAngle,iSize/2-5),
  				Sin(iAngle,100),-Cos(iAngle,100),
  				iSize*5,iColor, boom, 1);
}

global func Splatter(int iDmg, int iType, object pFrom, int iColor)
{
  var x,y;
  
  iDmg = iDmg*SplatterScale/100;

  if(iDmg < 10)
  {
    if(!Random(5-iDmg/2))
      return 0;//_inherited(iDmg,iType,pFrom);
    else
    {
      x = RandomX(-GetDefWidth(GetID())/3,+GetDefWidth(GetID())/3);
      y = RandomX(-GetDefHeight(GetID())/3,+GetDefHeight(GetID())/3);
    }
  }

  if(!iColor)
    iColor = RGB(80+Random(105));
  var size = BoundBy(iDmg,8,80);

  if(pFrom)
  {
    if((iType == DMG_Explosion)||
       (pFrom->~IsBullet()))
    {
      var angle = GetR(pFrom)-180;
      if(!angle) angle = Angle(GetX(),GetY(),GetX(pFrom),GetY(pFrom));
      //x = GetDefWidth()/2;
      //y = Tan(angle,x);
      x += Sin(angle,GetDefWidth()/2);
      y -= Cos(angle,GetDefHeight()/2);

      BloodSplatter2(Min(size*2,100),x,y,angle,iColor);
    }
  }
  BloodSplatter(Min(size*3,100),x,y,iColor);
  BloodBurst(Min(size*3,100),x,y,iColor);
}

/* Schadenseffekt */

global func AddDamageEffect(object target, int size)
{
  //Ziel muss die Funktion  GetDmgEffectPos(&x,&y) definiert haben
  if(!target) target = this;
  if(!target) return;

  AddEffect("DamageEffect", target, 1, 1, 0, 0, size); 
}

global func FxDamageEffectStart(object target, int number, int temp, size)
{
  if(!size) size = 100;
  EffectVar(0,target,number) = size; 
}

global func FxDamageEffectTimer(object target, int number, int time)
{
  //Wenn noch nicht ausreichend beschädigt, lassen
  var dmg = GetDamage(target);
  var maxdmg = target->~MaxDamage();

  if(dmg < maxdmg/2) return;

  var x, y, size = EffectVar(0,target,number);
  target->~GetDmgEffectPos(x,y);

  var xdir = +RandomX(-10,10);
  var ydir = -RandomX(-15,10);
  var smoke = RGBa(220,180,110,BoundBy(maxdmg-dmg,20,255));
  var thrust = RGBa(255,200,200,BoundBy(maxdmg-dmg,40,255));

  CreateParticle("Smoke2",GetX(target)+x,GetY(target)+y,xdir,ydir,RandomX(80*size/100,380*size/100),smoke);
  CreateParticle("Thrust",GetX(target)+x,GetY(target)+y,xdir,ydir,RandomX(120*size/100,size*2),thrust);
}

/* Bereichspartikel */

/*
  Erstellt im Rechteck "iPosition" "times"*"iAmount" Partikel des Typs "szName" "step"-mal der Größe "iSize.

  string sName			- Name des Partikels
  array aAmount[min, max]	- Bereich der Anzahl der Partikel
  array aPosition[x,y,w,h]	- Zielrechteck der Partikel
  array aTime[step, times]	- Zeitraum zwischen zwei Wiederholungen / Anzahl der Wiederholungen
  array aSize[min, max]		- Bereich der Größe der Partikel
  int iColor			- Farbe der Partikel
*/

global func AddParticlesInRect(string sName, array aAmount, array aPosition, array aTime, array aSize, int iColor)
{
  var iEffect = AddEffect("ParticlesInRectEffect",0,200,aTime[0],0,0);
  EffectVar(0, 0, iEffect) = sName;
  EffectVar(1, 0, iEffect) = aAmount[0];	//Min-Wert
  
  if(aAmount[0] < aAmount[1])
    EffectVar(2, 0, iEffect) = aAmount[1];	//Max-Wert
  else 
    EffectVar(2, 0, iEffect) = aAmount[0];
    
  EffectVar(3, 0, iEffect) = aPosition[0];	//X-Position
  EffectVar(4, 0, iEffect) = aPosition[1];	//Y-Position
  EffectVar(5, 0, iEffect) = aPosition[2];	//Weite
  EffectVar(6, 0, iEffect) = aPosition[3];	//Höhe
  EffectVar(7, 0, iEffect) = aSize[0];		//Min-Größe

  if(aSize[0] < aSize[1])
    EffectVar(8, 0, iEffect) = aSize[1];	//Max-Größe
  else 
    EffectVar(8,0, iEffect) = aSize[0];

  EffectVar(9, 0, iEffect) = aTime[1];		//Anzahl Wiederholungen
  EffectVar(10, 0, iEffect) = iColor;		//Farbe der Partikel

  var iLoop = aAmount[0] + Random(aAmount[1] - aAmount[0]);
  for(var i=0; i < iLoop; i++)
    CreateParticle(sName, aPosition[0] + Random(aPosition[2]), aPosition[1] + Random(aPosition[3]), 0, 0, aSize[0] + Random(aSize[1] - aSize[0]), iColor);

  return(1);
}

global func FxParticlesInRectEffectTimer(szNewEffect, iEffectNumber, iEffectTarget, iNewEffectNumber)
{
  var sName = EffectVar(0, 0, iEffectNumber);
  var iMinAmount = EffectVar(1, 0, iEffectNumber);
  var iMaxAmount = EffectVar(2, 0, iEffectNumber);
  var iX = EffectVar(3, 0, iEffectNumber);
  var iY = EffectVar(4, 0, iEffectNumber);
  var iW = EffectVar(5, 0, iEffectNumber);
  var iH = EffectVar(6, 0, iEffectNumber);
  var iMinSize = EffectVar(7, 0, iEffectNumber);
  var iMaxSize = EffectVar(8, 0, iEffectNumber);
  var iColor = EffectVar(10, 0, iEffectNumber);

  if(EffectVar(9, 0, iEffectNumber))
    EffectVar(9, 0, iEffectNumber)--;
  else 
    return -1;

  var iLoop = iMinAmount + Random(iMaxAmount - iMinAmount);
  for(var i=0; i < iLoop; i++)
    CreateParticle(sName, iX + Random(iW), iY + Random(iH), 0, 0, iMinSize + Random(iMaxSize - iMinSize), iColor);
  return 1; 
}