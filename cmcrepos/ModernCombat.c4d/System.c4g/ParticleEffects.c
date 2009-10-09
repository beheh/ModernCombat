/* Partikeleffekte */

#strict

global func MuzzleFlash(int iSize, object pClonk, int iX, int iY, int iAngle, int iColor)
{
  if(!pClonk) pClonk = this();
  
  var mx = +Sin(iAngle,iSize/4),
      my = -Cos(iAngle,iSize/4);

  CreateParticle("FireSpark",iX,iY,0,0,
                 iSize*3,iColor,pClonk);
                 
  CreateParticle("FireSpark",iX+mx/2,iY+my/2,0,0,
                 iSize*2,iColor,pClonk);
                 
  CreateParticle("FireSpark",iX+mx,iY+my,0,0,
                 iSize*1,iColor,pClonk);
                 
  //SmokeBurst(iSize*2/3,iX,iY,iAngle);
                 
  if(!IsDark())
    return();
    
  if(!iColor)
    iColor = RGB(255,255,255);
  
  var r,g,b,a;
  SplitRGBaValue(iColor,r,g,b,a);
  iColor = RGBa(r,g,b,Min(a+65,255));
  
  AddLightFlash(iSize*25, iX, iY, iColor);
}

global func SmokeBurst(int iSize, int iX, int iY, int iAngle, object pAttach, int dwColor)
{
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

global func BloodBurst(int iSize, int iX, int iY, int iColor)
{
	CreateParticle("BloodBurst",iX,iY,0,1,iSize*5,iColor);
}

global func BloodSplatter(int iSize, int iX, int iY, int iColor)
{
	// not on sky
	if(GetMaterialVal("Density","Material",GetMaterial(iX,iY)) != 0
       || GetMaterial(iX,iY) == -1) return();
	
	// behind everything (like the burn mark of the explosion) *cough*
	var boom;
	if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,0,0,-1);
	else boom = FindObject(BOOM);
	
	var r = Random(360);
	var xdir = Sin(r,100);
	var ydir = -Cos(r,100);
	CreateParticle("BloodSplatter",iX,iY,xdir,ydir,iSize*5,iColor,boom,1);
}

global func BloodSplatter2(int iSize, int iX, int iY, int iAngle, int iColor)
{
	if(GetMaterialVal("Density","Material",GetMaterial(iX,iY)) != 0
       || GetMaterial(iX,iY) == -1) return();

  iAngle += 180;

	var boom;
	if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,0,0,-1);
	else boom = FindObject(BOOM);

  CreateParticle("BloodSplatter2",iX+Sin(iAngle,iSize/2-5),iY-Cos(iAngle,iSize/2-5),
                                  Sin(iAngle,100),-Cos(iAngle,100),
                                  iSize*5,iColor, boom, 1);
}

static const SplatterScale = 50;
global func Splatter(int iDmg, int iType, object pFrom, int iColor)
{
  //if(!GetAlive(this())) return();
  var x,y;
  
  iDmg = iDmg*SplatterScale/100;

  if(iDmg < 10)
  {
    if(!Random(10-iDmg))
      return _inherited(iDmg,iType,pFrom);
    else
    {
      x = RandomX(-GetDefWidth(GetID())/3,+GetDefWidth(GetID())/3);
      y = RandomX(-GetDefHeight(GetID())/3,+GetDefHeight(GetID())/3);
    }
  }

	if(!iColor)
    iColor = RGB(150+Random(105));
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


//Von Hazard, aber etwas erweitert. :)
global func AddDamageEffect(object target, int size)
{
	// target muss die Funktion  GetDmgEffectPos(&x,&y) definiert haben
	// und MaxDamage
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
	// wenn noch nicht ausreichend beschädigt, lassen
	var dmg = target->GetDamage();
	var maxdmg = target->~MaxDamage();

	if(dmg < maxdmg/2) return;

	var x, y, size = EffectVar(0,target,number);
	target->~GetDmgEffectPos(x,y);
	
	// occupacy of the smoke is dependend on  damage taken
	var xdir = +RandomX(-10,10);
	var ydir = -RandomX(-15,10);
	var smoke = RGBa(220,180,110,BoundBy(maxdmg-dmg,20,255));
	var thrust = RGBa(255,200,200,BoundBy(maxdmg-dmg,40,255));

	CreateParticle("Smoke2",target->GetX()+x,target->GetY()+y,xdir,ydir,RandomX(80*size/100,380*size/100),smoke);
	CreateParticle("Thrust",target->GetX()+x,target->GetY()+y,xdir,ydir,RandomX(120*size/100,size*2),thrust);
}