/*-- Rakete --*/

#strict
#include MISS

public func Acceleration()	{return(3);}
public func MaxTime()		{return(200);}
public func MaxSpeed()		{return(100);}
protected func SecureDistance()	{return(100);} //Mindestabstand

local sx,sy,start;

/* Start */

public func Launch(int iAngle, int iDmg, object pFollow)
{
  //Werte setzen
  iSpeed = 5;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");
  
  SetPlrViewRange(120);
  SetCategory(C4D_Vehicle);
  
  sx = GetX();
  sy = GetY();
  start = FrameCounter();

  //Effekte
  AddEffect("ThrustSound",this(),1,11,this());
  AddLight(70,RGB(255,200,200),this(),GLOW);

  //Treffer- und Steuereffekt einsetzen
  AddEffect("HitCheck", this(), 1,1, 0, SHT1,shooter);
  if(pFollow)
   AddEffect("Follow", this(), 1,1, 0, GetID(),pFollow);
}

protected func Secure()
{
  if(Distance(GetX(),GetY(),sx,sy) <= SecureDistance() && FrameCounter() < start+70)
   return(true);

  return(false);
}

/* Soundeffekt */

public func FxThrustSoundTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  Sound("RPGP_Thrust.ogg",0,0,0,0,+1);
  return(-1);
}

/* Optischer Steuerungseffekt */

public func FxFollowStart(object pTarget, int iEffectNumber, int iTemp, obj)
{
  if(!obj) return(-1);
  EffectVar(0,pTarget,iEffectNumber) = obj;
  EffectVar(1,pTarget,iEffectNumber) = 0;
}

public func FxFollowTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Ziel noch gültig?
  if(EffectVar(1,pTarget,iEffectNumber)) {
    var pEnemy = EffectVar(1,pTarget,iEffectNumber);
    var del = false;
  	if(!GetEffect("TracerDart", pEnemy)) del = true;
		if(!PathFree(GetX(pTarget), GetY(pTarget), GetX(pEnemy), GetY(pEnemy))) del = true;
		if(del) EffectVar(1,pTarget,iEffectNumber) = 0;
  }
  //Haben wir noch ein Ziel?
  if(!EffectVar(1,pTarget,iEffectNumber)) {
  	var pEnemies = FindObjects(Find_Distance(300, GetX(pTarget), GetY(pTarget)), Sort_Distance(GetX(pTarget), GetY(pTarget)));
		for(var pEnemy in pEnemies) {
		  var iEffectTracer = GetEffect("TracerDart", pEnemy);
			if(!iEffectTracer) continue;
			var iPlr = EffectVar(0, pEnemy, iEffectTracer);
      if(Hostile(GetController(pTarget), iPlr)) continue;
			if(!PathFree(GetX(pTarget), GetY(pTarget), GetX(pEnemy), GetY(pEnemy))) continue;
		  EffectVar(1, pTarget, iEffectNumber) = pEnemy;
      Sound("BBTP_Alarm.ogg", false, this);
		  break;
		}
  }
  //Soll-Winkel
  var iDAngle;
  var iMaxTurn;
  //Sonst anvisieren!
  if(EffectVar(1,pTarget,iEffectNumber)) {
  	var pEnemy = EffectVar(1,pTarget,iEffectNumber);
  	iDAngle = Angle(AbsX(GetX(pTarget)), AbsY(GetY(pTarget)), AbsX(GetX(pEnemy)), AbsY(GetY(pEnemy)));
  	iMaxTurn = 8;
  }
  else {
    var obj = EffectVar(0,pTarget,iEffectNumber);
		if(!obj)
		 return;
		//Schütze nicht mehr am Zielen?
		if(!obj->~IsAiming())
		 return;

		iDAngle = obj->AimAngle();
		iMaxTurn = 6;
  }
	var iAngle = GetR(pTarget);

	var iDiff = Normalize(iDAngle - iAngle,-180);
	var iTurn = Min(Abs(iDiff),iMaxTurn);

	pTarget->SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}

private func Traveling()
{
  if(GetActTime() >= MaxTime()) return(Hit());
  //Geschwindigkeit erhöhen
  Accelerate();
  //Rauchspur
  Smoking();
}

private func StopThrust()
{
  Sound("RPGP_Thrust.ogg",0,0,0,0,-1);
}

/* Rauch */

private func Smoking()
{
  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist)
  {
   var x = -maxx*i/dist;
   var y = -maxy*i/dist;

   var rand = RandomX(-10,10);
   var xdir = +Sin(GetR()+rand,8);
   var ydir = -Cos(GetR()+rand,8);

   CreateParticle("Smoke3",x,y,xdir,ydir,RandomX(50,70),RGBa(255,255,255,85),0,0);
  }

  CreateParticle("MuzzleFlash2",-maxx,-maxy,+Sin(GetR()+180,500),-Cos(GetR()+180,500),RandomX(80,140),RGBa(255,200),this());
}

/* Schaden */

public func Damage()
{
  //Rakete abschießbar
  if(GetDamage() > 5 && !exploding)
  {
   SetAction("Idle");
   FadeOut4K(4);
   AddEffect("Damaged",this(),1,1,this());
   Sound("RPGP_ShotDown.ogg");
  }
}

public func FxDamagedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->CreateParticle("Thrust",0,0,0,0,70+Random(30),RGBa(255,255,255,0),0,0);
}

public func Hit()
{
  if(GetAction() eq "Idle")
  {
   Explode(3,0,0,0,1);
   CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  }
  else
  {
   HitObject();
  }
}

private func HitObject(pObj)
{
  if(GetAction() eq "Idle")
   return Explode(3,0,0,0,1);

  if(Secure())
  {
   if(pObj)
   {
    DoDmg(Distance(GetXDir(),GetYDir())/5,DMG_Projectile,pObj);
    CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    if(GetOCF(pObj) & OCF_Living)
    {
     Sound("SharpnelImpact*.ogg");
     Fling(pObj,GetXDir()/15,GetYDir()/15-1);
    }
    else
    {
     Sound("BlockOff*.ogg");
     Sparks(30,RGB(255,128));
     CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    }
    RemoveObject();
    return();
   }
   else
   {
    Sound("GrenadeHit*.ogg");
    Sparks(30,RGB(255,128));
    CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    RemoveObject();
    return();
   }
  }

  exploding = true;
  Sound("GrenadeExplosion*.ogg");

  //Schaden verursachen
  DamageObjects(iDamage,iDamage/2,this());
  Explode(iDamage*3/2,0,0,0,1);
}

public func Destruction()
{
  StopThrust();
}
