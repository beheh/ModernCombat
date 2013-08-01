/*-- Splitter --*/

#strict 2
#include SHTX

local hitcnt, size, trail_len, iAttachment;

public func IsSpecialAmmo()	{return false;}


/* Abschuss */

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int attachment)
{
  //Schaden des Splitters setzen
  if(!iDmg)
    iDamage = 3;
  else
    iDamage = iDmg;

  //Attachment setzen
  iAttachment = attachment;

  //Position des Splitters setzen
  SetPosition(GetX(),GetY()+GetDefWidth()/2);

  //Größe und Länge setzen
  if(!iSize)
    iSize = 8;
  if(!iTrail)
    iTrail = 300;

  //Geschwindigkeit setzen
  speed = iSpeed;

  iSize = Min(iSize+2,GetDefWidth());

  DoCon(100*iSize/GetDefWidth()-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*iDist/iSpeed;

  if(!iTime)
    return RemoveObject();

  var self = this;
  SetAction("Travel");

  //Sicherheitscheck, ob der Splitter nicht sofort verschwindet
  if(!self) return ;

  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(+iAngle);

  //Trail erzeugen
  size = iSize;
  trail_len = iTrail;
  CreateTrail(iSize, iTrail);

  AddEffect("HitCheck", this, 1,1, 0, GetID(), shooter);
}

/* Im Flug */

private func CreateTrail(int iSize, int iTrail)
{
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize-2,iTrail,this);
    SetGraphics("Trail",pTrail,GetID());

    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func Traveling()
{
  var iATime = GetActTime();

  //Ausfaden
  SetClrModulation(Color(iATime));

  //Flugzeit aufgebraucht: Verschwinden
  if(iATime >= iTime) return Remove();

  //Außerhalb der Landschaft: Verschwinden
  if(GetY()<0) return Remove();
}

private func Color(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return RGBa(255-iPrg*2,255-iPrg*2,255-iPrg*2,iPrg*2);
}

public func TrailColor(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return RGBa(255-iPrg*2,255-iPrg*2,255-iPrg*2,iPrg*2);
}

/* Treffer und Aufprall */

public func Hit(int iXDir, int iYDir)
{
  if(!iXDir && !iYDir)
  {
    iXDir = GetXDir(0, 100);
    iYDir = GetYDir(0, 100);
  }

  if(!Random(GetActTime()/2))
  {
    //Hazard-Script
    if(iXDir != 0)
    {
      if(iXDir > 0)
      {
        if(GBackSolid(1,0))
          iXDir *= -1;
      }
      else
        if(GBackSolid(-1,0))
          iXDir *= -1;
    }
    if(iYDir != 0)
    {
      if(iYDir > 0)
      {
        if(GBackSolid(0,1))
          iYDir *= -1;
      }
      else
        if(GBackSolid(0,-1))
          iYDir *= -1;
    }

    SetXDir(iXDir, 0, 100);
    SetYDir(iYDir, 0, 100);
    SetR(Angle(0, 0, iXDir, iYDir));

    SetPosition(GetX(),GetY(), pTrail);
    SetSpeed(0,0,pTrail);
    pTrail->Remove();

    CreateTrail(size,trail_len);

    return ;
  }
  return HitLandscape();
}

private func HitObject(object pObject)
{
  if(shooter && pObject)
    if(pObject == shooter)
      return false;//>:O

  if(BulletStrike(pObject))
  {
    hitcnt++;

    var x,y;
    OnShrapnelHit(pObject,x,y);

    if((hitcnt > 1) || !pObject)
      Remove();
    return true;
  }
}

private func HitLandscape()
{
  OnHitLandscape();

  //Umliegende Objekte beschädigen
  var objs = FindObjects(Find_AtPoint(), Find_NoContainer(), Find_Or(Find_Func("IsBulletTarget",GetID(),this,shooter), Find_OCF(OCF_Alive)), Find_Func("CheckEnemy",this), Find_Not(Find_Func(ObjectCall(this,"HitExclude"))));  
  {
    for(var pTarget in objs)
      HitObject(pTarget);
  }

  HitObject();
}

public func OnHitLandscape()
{
  Sound("BulletHit*.ogg");
  Sound("Crumble*.ogg");
}

public func BulletStrike(object pObj)
{
  if(pObj)
  {
    if(pObj->~IsBullet()) return false;
    if(GetID(pObj) == TRAI) return false;
    if(GetEffect("IntShrapnelHit",pObj))
    {
      return false;
    }

    if(GetID(pObj) != RSLH)
      AddEffect("IntShrapnelHit",pObj,1,10,0,GetID());

    if(GetOCF(pObj) & OCF_Alive)
    {
      //Fling(pObj,GetXDir()/20 + GetXDir(pObj),GetYDir()/20 + GetYDir(pObj));
      ObjectSetAction(pObj, "Tumble");
    }

    if(GetID(pObj) == RSLH)
    {
      DoDmg(1,DMG_Projectile,pObj,0,0,wpnid, iAttachment);
    }
    else
    {
      DoDmg(iDamage,DMG_Projectile,pObj,0,0,wpnid, iAttachment);
    }

    return true;
  }
  return true;
}

public func FxIntShrapnelHitTimer()	{return -1;}

public func OnShrapnelHit(object pObject, int iX, int iY)
{
  if(pObject)
  {
    if(Distance(lx,ly) > 20)
      Sound("SharpnelImpact*.ogg");
  }
}

/* Effekt für Trefferüberprüfung */

// EffectVars:
// 0 - alte X-Position
// 1 - alte Y-Position
// 2 - Schütze (Objekt, das die Waffe abgefeuert hat, üblicherweise ein Clonk)
// 3 - ID des Schützen
// 4 - Scharf? Wenn true wird der Schütze vom Projektil getroffen 
// 5 - niemals den Schützen treffen

public func FxHitCheckStart(object target, int effect, int temp, object byObj, bool neverShooter)
{
  if(temp) return;
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  if(!byObj)
    byObj = target;
  if(Contained(byObj))
    byObj = (Contained(byObj));
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = false;
  EffectVar(5, target, effect) = neverShooter;
}

public func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  // Oh man. :O
  var oldx = EffectVar(0, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  //Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  //Selbst nicht treffbar
  if(EffectVar(4, target, effect)) exclude = target;

  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (entfernten zuerst, da wir die zuerst passieren)
  for(obj in FindObjects(Find_OnLine(oldx,oldy,newx,newy),
                         Find_NoContainer(),
                         Sort_Distance(oldx, oldy)))
  {
    if(obj == target) continue;
    if(obj == exclude) continue;
    if(obj->~HitExclude(target)) continue;
    if(!CheckEnemy(obj,target)) continue;
    if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect), oldx, oldy) || GetOCF(obj) & OCF_Alive)
    {
      if(!target)
        return -1;
      DebugLog("%s IsBulletTarget: %i, %s, %s","HitCheck",GetName(obj),GetID(target),GetName(target),GetName(EffectVar(2, target, effect)));
      return target-> ~HitObject(obj);
    }
  }

  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  //Der Schuss wird erst "scharf gemacht", d.h. kann den Schützen selbst treffen, wenn der Schuss einmal die Shape des Schützen verlassen hat.

  if(!EffectVar(5,target,effect))
  {
    if(!EffectVar(4, target, effect))
    {
      //Scharf?
      var ready = true;
      //Check nach Objekten mit ID des Schützen an momentaner Stelle
      for(var foo in FindObjects(Find_AtPoint(GetX(target),GetY(target)),Find_ID(EffectVar(3, target, effect))))
      //Gefundenes der Schütze?
      if(foo == EffectVar(2, target, effect))
        ready = false;
      if(ready)
      //Treffbarkeit einstellen
      EffectVar(4, target, effect) = true;
    }
  }
}

public func Ricochet(int iX, int iY)
{
  if(GBackSolid()) return;
  if(iDamage <= 1) return;

  var I = Wrap4K(GetR(),0,360);
  var A = SurfaceNormal4K(iX,iY,2);
  var O = Wrap4K(Flip(A,I-180),0,360);
  var H = Wrap4K(OffsetX(O,I),0,360);

  /*Log("In:%d] Surface:%d] Abprallwinkel:%d] Reflektionswinkel:%d]",I,A,H,O);

  DrawParticleLine ("PSpark",0,0,Sin(I,100),-Cos(I,100),4,30,RGB(0,255),RGB(0,255));
  DrawParticleLine ("PSpark",0,0,Sin(A,100),-Cos(A,100),2,20,RGB(255),RGB(255));
  DrawParticleLine ("PSpark",0,0,Sin(O,100),-Cos(O,100),4,30,RGB(0,128,255),RGB(0,128,255));
  DrawParticleLine ("PSpark",0,0,Sin(H,100),-Cos(H,100),2,20,RGB(0,128,255),RGB(255,255,255));*/

  if(H <= RicochetAngle())//Winkel okay?
  {
    var O = A+(A-I)+180;
    iDamage = iDamage-iDamage*(H*50/RicochetAngle())/100;//Maximal 50% abziehen.
    Sound("Ricochet*.ogg");

    SetXDir(+Sin(O,speed));
    SetYDir(-Cos(O,speed));
    SetR(O);

    var effect = GetEffect("HitCheck",this);
    if(effect)
    {
      EffectVar(0,this,effect) = GetX();
      EffectVar(1,this,effect) = GetY();
    }

    var oldtrail = pTrail;
    CreateTrail(oldtrail->LocalN("w")/1000*20, oldtrail->LocalN("l")/1000*100);

    oldtrail->SetPosition(GetX()+iX,GetY()+iY);
    oldtrail->SetXDir();
    oldtrail->SetYDir();
    //oldtrail->RemoveObject();
    return true;
  }
}

public func Remove(int iRemoveTime)
{
  if(pTrail)
  {
    SetPosition(GetX(),GetY(),pTrail);
    pTrail->Remove();
  }

  RemoveObject();
}
