/* Kampfmesser */
#strict

public func HandX(){return(6000);}
public func IsEquipment(){return(true);}
public func IsDrawable(){return(true);}

private func KnifePower(){return(60);}
private func KnifePower2(){return(20);}
private func KnifeRange(){return(13);}
private func KnifeSpeed(){return(20);}
private func KnifeMaxDmg(){return(1000);}//Bei diesem Wert wird die maximale Blutigkeit des Messers erreicht. O_O' *schluck* ... AHhhhh!!!1111

local action,fast;

/* Angreifen */
//frag! frag! frag!
public func ControlThrow(pClonk)
{
  CloseCombat(pClonk);
  return(1);
}

public func CloseCombat(pClonk)
{
  if (action) return(1);
  fast = 1;

  if(!(pClonk->ReadyToFire()))
    return(1);
    
  var target,offx;
  if(GetDir(pClonk)== DIR_Left()) offx = -1;
                             else offx =  1;

  if(GetAction(pClonk) eq "WalkArmed")
    if(!GetComDir(pClonk))
    {
      fast = 0;
      ObjectSetAction(pClonk,"Throw");
    }

  for(var target in FindObjects(Find_InRect(offx*KnifeRange(),-5,-(offx*KnifeRange()),10), 
                        Find_Func("IsBulletTarget"), 
                        Find_Exclude(pClonk), 
                        Find_OCF(OCF_Alive)))                          
    if(!(CheckEnemy(target,pClonk)))
      continue;

  if(target)
  {
    if(fast)
      HitObject(target,KnifePower2(),pClonk);
    else
      HitObject(target,KnifePower(),pClonk);
  }
  else
  {
    if(GBackSolid(offx*10,0))
    {
      Sound("KN_WALL",0,0,fast,0,0,1);
      AddLightFlash(20,offx*5,0,RGB(255,255,180));
      Sparks (Random(3),RGB(255,255,0),offx*5,0);
    }
    else
      Sound("KN_SLASH",0,0,fast,0,0,2);
  }

  AttackStart();

  return(1);
}

private func AttackStart()
{
  action = true;
  ScheduleCall (0,"AttackEnd",KnifeSpeed());
}

private func AttackEnd()
{
  action = false;
}

private func HitObject(pTarget,iDamage,pClonk)
{  
  if(!pTarget) return(0);

  Sound("KN*",pTarget,0,100,0,0,Random(3)+1);
  //MakeBloody(iDamage,pTarget);
  DoDmg (iDamage,DMG_Melee,pTarget);
  return(1);
}

private func MakeBloody(iDamage,pTarget)
{
  SetGraphics ("Blood",0,GetID(),1,GFXOV_MODE_Picture,0);

  var br,bg,bb,ba;
  SplitRGBaValue (GetBloodColour(pTarget),br,bg,bb,ba);

  var r,g,b,a; 
  SplitRGBaValue (GetClrModulation(0,1),r,g,b,a);

  r -= 255-br;
  g -= 255-bg;
  b -= 255-bb;

  if(r < 0)
    r = 0;
  if(g < 0)
    g = 0;
  if(b < 0)
    b = 0;
  
  //a -= (KnifeMaxDmg()/255)*iDamage;

  var rgba;
  rgba = SetRGBaValue (rgba,a,0);
  rgba = SetRGBaValue (rgba,r,1);
  rgba = SetRGBaValue (rgba,g,2);
  rgba = SetRGBaValue (rgba,b,3);

  SetClrModulation (rgba,0,1); 
}

private func ClearBlood(rgb)
{
  var br,bg,bb,ba;
  SplitRGBaValue (rgb,br,bg,bb,ba);

  var r,g,b,a; 
  SplitRGBaValue (GetClrModulation(0,1),r,g,b,a);  

  a += 10;
  r += 10;
  g += 10;
  b += 10;
  
  if(a > 255)
    a = 255;
  if(r > 255)
    r = 255;
  if(g > 255)
    g = 255;
  if(b > 255)
    b = 255;
}

private func ClearBlood()
{
  var r,g,b,a; 
  SplitRGBaValue (GetClrModulation(0,1),r,g,b,a);  

  a += 10;
  
  if(a > 255)
  {
    a = 255;
    r = 255;
    g = 255;
    b = 255;
  }
}

/* Effekte */

protected func Hit()
{
  Sound("MetalHit*");
  return(1);
}

public func Unholster()
{
  Sound("KN_DEPLOY");
  return(1);  
}
