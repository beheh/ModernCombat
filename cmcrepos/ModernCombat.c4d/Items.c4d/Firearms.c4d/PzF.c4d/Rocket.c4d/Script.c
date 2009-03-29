/* Hochexplosivrakete */

#strict
#include MISS

public func Acceleration() { return(7); }
public func MaxTime() { return(120); }
public func MaxSpeed() { return(220); }

private func Traveling()
{
  // kommt nich weiter
  if(GetActTime() >= MaxTime()) return(Hit());

  // beschleunigen
  Accelerate();

  // Effekte
  Smoking();
  
  if(GetR()<0)   SetRDir(-10,this(),100);
  if(GetR()>0)   SetRDir(+10,this(),100);
}

private func Smoking()
{
  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;
  
  CreateParticle("MuzzleFlash2",0,0,+Sin(GetR()+180,500),-Cos(GetR()+180,500),RandomX(20,80)*5,RGBa(255,57),this());

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

private func HitObject(object pObject)
{
  DamageObjects(iDamage*4/5,iDamage/2,this());
  inherited(pObject);
}