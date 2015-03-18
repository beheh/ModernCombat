/*-- EMP-Bolzen --*/

#strict 2
#include GREN

local active,sx,sy, start;
local iXDir, iYDir;
local iLastAttacker;
local iAttachment;
local fStuck;
local fHit;

public func Color()		{return RGB(255,0,0);}
public func IgnoreTracer()	{return true;}
public func IsArrow()		{return true;}
public func AllowHitboxCheck()	{return true;}
public func RejectC4Attach()	{return true;}
public func IsBulletTarget()	{return false;}


/* Initialisierung */

protected func Initialize()
{
  iLastAttacker = NO_OWNER;
  return _inherited();
}

/* Start */

func Launch(int xdir, int ydir, int iDmg,a,b,c, int attachment)
{
  iAttachment = attachment;
  SetCategory(C4D_Vehicle);
  active = true;
  sx = GetX();
  sy = GetY();
  start = FrameCounter();
  iXDir = xdir;
  iYDir = ydir - 7;
  AddEffect("HitCheck", this(), 1, 1, 0, SHT1,shooter);
  AddEffect("Grenade", this, 1, 1, this);
  SetSpeed(xdir, ydir);
  iDamage = iDmg;
  if(Stuck()) Hit();
}

/* Timer */

func FxGrenadeTimer(object target, int effect, int time)
{
	if(fStuck)
	{
		SetXDir(0);
		SetYDir(-2);
		SetRDir(0);
	}
	else
  	SetR(Angle(0,0,GetXDir(),GetYDir()));
}

/* Treffer */

func Hit()
{

	var x, y, xdir, ydir, fFound;
	x = GetX();
 	y = GetY();

	xdir = GetXDir();
	ydir = GetYDir();

	SimFlight(x, y, xdir, ydir);

	x -= GetX();
	y -= GetY();

	//var normX, normY;

	for(var i = -1; i < 2; i++)
  {
    for(var j = -1; j < 2; j++)
    {
      if(GetMaterialVal("DigFree", "Material", GetMaterial(x + i, y + j)))
      {
        fFound = true;
        break;
      }/*
      if(GetMaterial(x + i, y + j) == -1)
      {
      	normX = i;
        normY = j;
      }*/
    }
    if(fFound)
      break;
  }

	x += GetX();
	y += GetY();
	
  if(fFound)
  {
    fStuck = true;
    SetPosition(x, y);
    SetXDir(0);
    SetYDir(-2);
    SetRDir(0);
  }/*
  else
  {
  	if(!fHit)Log("%d %d %d %d", normX, Sgn(xdir), normY, Sgn(ydir));
  	//SetPosition(x, y);
  	if(normX == Sgn(xdir))
  		SetXDir(-xdir/2);
  	if(normY == Sgn(ydir))
  		SetYDir(-ydir/2);
  	
  	SetRDir(Abs((xdir+ydir))*10);  	
  }*/
	//else
	//	Log("x: %d, y: %d xdir: %d, ydir: %d", GetX()+GetVertex(0, false)+ Sgn(GetXDir()), GetY()+GetVertex(0, true)+ Sgn(GetYDir()), GetXDir(), GetYDir());
	
	if(!fHit)
  	FadeOut(this);
  		
	fHit = true;
}

protected func RejectEntrance(pNewContainer)
{
  return 1;
}

func HitObject(object pObj)
{
	if(fHit)
		return;

  if(pObj)
  {
    DoDmg(40, DMG_Energy, pObj, 0, 0, 0, iAttachment);

    //if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
    if(GetOCF(pObj) & OCF_Living)
    {
      Sound("SharpnelImpact*.ogg");
      ObjectSetAction(pObj, "Tumble");
    }
    else
    {
      Sound("BulletHitMetal*.ogg");
      Sparks(30,RGB(255,128));
    }
    RemoveObject();
    return;
  }/*
  else
  {
    Sound("GrenadeHit*.ogg");
    Sparks(30,RGB(255,128));
    //if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
    RemoveObject();
    return;
  }*/

  if(Hostile(iLastAttacker, GetController()) && GetID() != ERND)
    //Punkte bei Belohnungssystem (Granatenabwehr)
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, iLastAttacker, GetCursor(iLastAttacker), IC16);
}

func Damage()
{
	if(fHit)
		return;
		
  if(GetDamage() > 10)
  {
    //Effekte
    CastParticles("MetalSplinter",3,80,0,0,45,50,RGB(40,20,20));
    Sparks(8,RGB(255,128));
    Sound("BulletHitMetal*.ogg");
  }
}

/* Schaden */

public func OnHit(a, b, pFrom)
{
  iLastAttacker = GetController(pFrom);
  return;
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 100;	//Feuer
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}
