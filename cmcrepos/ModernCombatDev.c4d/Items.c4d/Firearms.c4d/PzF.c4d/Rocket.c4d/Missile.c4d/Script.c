/* Rakete */

#strict 2

local iSpeed, iDamage, exploding;
local shooter;

public func Acceleration() { return 5; }
public func MaxTime() { return 60; }
public func MaxSpeed() { return 180; }

func Construction(object byObj)
{
  // nichts? :C
  if(!byObj)
    return ;
  // Waffe?
  if(byObj->~IsWeapon())
    // User der Waffe?
    if(!(shooter = byObj->GetUser()))
      // kein User -> Waffe.
      shooter = byObj;
  
  // noch immer nichst gefunden?
  if(!shooter)
    // dann einfach byObj nehmen.
    shooter = byObj;
}

public func Launch(int iAngle, int iDmg)
{
  iSpeed = 20;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");

  AddLight(100,RGB(255,127,0),this,GLOW);

  AddEffect("HitCheck", this, 1,1, 0, SHT1,shooter);
}

/* Timer */

private func Accelerate()
{ 
  if(iSpeed < MaxSpeed())
    iSpeed += Acceleration();

  SetXDir(+Sin(GetR(),iSpeed));
  SetYDir(-Cos(GetR(),iSpeed));
}

private func Smoking()
{
  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;
  
  CreateParticle("MuzzleFlash2",0,0,+Sin(GetR()+180,500),-Cos(GetR()+180,500),RandomX(20,80)*5,RGBa(255,57),this);

  for(var i=0; i<dist; i+=ptrdist)
  {

    var x = -maxx*i/dist;
    var y = -maxy*i/dist;


    var rand = RandomX(-10,10);
    var xdir = +Sin(GetR()+rand,8);
    var ydir = -Cos(GetR()+rand,8);
    
    var size = RandomX(70,90);

    CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,size,RGBa(255,200,200,60),0,0);
    CreateParticle("Smoke3",x,y,xdir,ydir,size,RGBa(255,255,255,128),0,0);
  }
}

private func Traveling()
{
  // kommt nich weiter
  if(GetActTime() >= MaxTime()) return Hit();

  // beschleunigen
  Accelerate();

  // Effekte
  Smoking();
  
  SetRDir(RandomX(-30,+30),this,100);
}
  
/* Treffer */

private func Hit()
{
  HitObject();
}

private func HitObject(object pObject)
{
  exploding = true;
  DamageObjects(iDamage,iDamage/2,this());
  Explode(iDamage,0,0,0,1);
}

public func Damage()
{
  if(GetDamage() > 20 && !exploding)
    Hit();
}

public func EMPShock()
{
  SetAction("Idle");
}

public func IsBulletTarget() { return true; }
public func NoDecoDamage() { return true; }
public func IsMachine() { return true; }
