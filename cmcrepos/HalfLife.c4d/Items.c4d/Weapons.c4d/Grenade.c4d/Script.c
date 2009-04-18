/*-- Granate --*/

#strict
#include NADE

public func FuseTime(){return(3*35);}
public func Color(){return(RGB(255));}

func HitSound()
{
  Sound("SGRN_Hit*");
}

public func Fused()
{
  //Macht ordentlich Krawumm. :I
  Sound("GrenadeExplosion*.ogg");
  DamageObjects(40,170,this());
  Explode(40);
}

public func FxIntFuseStart(object pTarget, int iEffectNumber)
{
  activated = true;
  
  EffectVar(0,pTarget,iEffectNumber) = GetX();
  EffectVar(1,pTarget,iEffectNumber) = GetY();
  
  return(1);
}

public func FxIntFuseTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var xoff = 0,yoff = 0;
  
  if(!(iEffectTime % 35))
    Sound("H2GR_Tick.ogg");

  if(!Contained())
  {
    GetOffset4K(xoff,yoff);
  
    if(Distance(GetXDir(),GetYDir()) > 10)
      DrawParticleLine("PSpark",GetX()-EffectVar(0,pTarget,iEffectNumber),GetY()-EffectVar(1,pTarget,iEffectNumber),
                       xoff,yoff,3,30,RGBa(255,0,0,100),RGBa(255,0,0,100));
    
    if(!(iEffectTime % 35))
      CreateParticle("PSpark",xoff,yoff,0,0,60,RGB(255),this());
  }
  else
  {
    if(Contained()->Contents() == this())
      PlayerMessage(GetController(Contained()),"<c %x>•</c>",Contained(),InterpolateRGBa2(RGB(0,255),RGB(255,255),RGB(255,0),0,FuseTime(),iEffectTime));    
  }
  
  EffectVar(0,pTarget,iEffectNumber) = GetX()+xoff;
  EffectVar(1,pTarget,iEffectNumber) = GetY()+yoff;

  if(iEffectTime < FuseTime()) return();
  
  var c = Contained();
  if(Contained())
  {
    if(GetID(Contained()) == GRNS)
    {
      var obj = Contained();
      var user = obj->GetUser();
      if(!user)
        return(false);
        
      Exit();
      SetPosition(GetX(user),GetY(user));
    }
    else
      Exit();
  }
  pTarget->Fused2(c);
  return(-1);
}