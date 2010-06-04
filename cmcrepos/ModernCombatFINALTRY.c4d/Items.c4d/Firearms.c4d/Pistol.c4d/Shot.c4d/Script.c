/*-- Schuss --*/

#strict

local iTime, lx, ly, pTrail, iDamage,speed,max_dst,dst,fb;
local shooter,wpnid; // Dingens/Clonk das den Schuss abgefeuert hat.


func Construction(object byObj)
{
  // nichts? :C
  if(!byObj)
    return();
    
  //WaffenID?
  wpnid = GetID(byObj);
    
  //Waffe?
  shooter = GetShooter(byObj);
  
  //Team?
  if(shooter->GetTeam())
  	SetTeam(shooter->GetTeam());
}

public func Initialize()
{
  SetObjectBlitMode(1);
}

public func Fast()
{
  return(FastBullets() || fb);
}

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime)
{
  if(Fast())
  {
    return(LaunchFB(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime,...));
  }

  if(!iDmg)     iDamage = 3;
  else iDamage = iDmg;

  SetPosition(GetX(),GetY()+GetDefWidth()/2);

  if(!iSize)    iSize = 8;
  if(!iTrail)   iTrail = 300;
  
  speed = iSpeed;

  iSize = Min(iSize+2,GetDefWidth());


  DoCon(100*iSize/GetDefWidth()-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*iDist/iSpeed;
  
  if(!iTime)
    return(RemoveObject());

  var self = this();
  SetAction("Travel");
  if(!self) return();   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(+iAngle);

  // Trail erzeugen
  CreateTrail(iSize, iTrail);

  AddEffect("HitCheck", this(), 1,1, 0, GetID(), shooter);
}

public func LaunchFB(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime)
{
  if(!iDmg) iDamage = 3;
  else iDamage = iDmg;

  SetPosition(GetX(),GetY()+GetDefWidth()/2);
  
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(iAngle);//Vielleicht wird das iwo benutzt?
  DoCon(100*iSize/GetDefWidth()-100);//Und das?
  
  SetAction("Travel");
  
  max_dst = iDist;
  speed = iSpeed;
  iTime = 1;
  fb = true;
  
  //Jetzt wirds spannend. :>
  HitCheck(iAngle,iDist);

  /*if(iSize)
  {
    if(dst < max_dst)
    {
      var t = CreateObject(TRAI,lx,ly,-1);
      if(t)
      {
        t->SetFB(iSize,iTrail,this());
        t->SetObjectBlitMode(GetObjectBlitMode());
      }
    }
    else
    {
      if(!Random(2))
      {
        var t = CreateObject(TRAI,0,0,-1);
        if(t)
        {
          t->SetFB(iSize,iTrail,this(),iDist);
          t->SetObjectBlitMode(GetObjectBlitMode());
        }
      }
    }
  }*/
  
  //SetVisibility(VIS_None);
  
  Remove(iRemoveTime);
}

//Für bessere Richtungsbestimmung. :>
public func SetAngle(int iAngle, int iAPrec)
{
  if(!iAPrec) iAPrec = 1;
  SetXDir(+Sin(iAngle,speed,iAPrec));
  SetYDir(-Cos(iAngle,speed,iAPrec));
  SetR(iAngle/iAPrec,0);
}

// Extern für Überladung
private func CreateTrail(int iSize, int iTrail)
{
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize-2,iTrail,this());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Timer */

private func Traveling()
{
  if(Fast()) return();

  var iATime = GetActTime();

  // ausfaden
  SetClrModulation(Color(iATime));
  // löschen
  if(iATime >= iTime) return(Remove());
  
  // außerhalb der Landschaft: löschen
  if(GetY()<0) return(Remove());
}


/* Querschläger usw. */

public func RicochetAngle()//maximaler Abprallwinkel
{
  if(GetID() != SHTX) return();
  return(50);
}

public func OffsetX(int iA, int iB)
{
  var off = Offset(iA,iB);
  if(off > 180)
  {
    var da = -iA;
    var db = -iB;
    
    if(iA > 180)
      da = 360-iA;

    if(iB > 180)
      db = 360-iB;
      
    iA = 180+db;
    iB = 180+da;
      
    off = Offset(iA,iB);
  }
  
  return(off);
}

public func Offset(int iA, int iB)
{
  if(iA > iB)
    return(iA-iB);
  else
    return(iB-iA);
}

public func Flip(int iA, int iB)
{
  var off = OffsetX(iA,iB);
  if(iA > iB)
    return(iA+off);
  return(iA-off);
}

public func Ricochet(int iX, int iY)
{
  if(GBackSolid()) return();
  if(iDamage <= 1) return();

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
    
    if(Fast())
    {
      HitCheck(O,max_dst-dst);
    }
    else
    {
      SetXDir(+Sin(O,speed));
      SetYDir(-Cos(O,speed));
      SetR(O);
      
      var effect = GetEffect("HitCheck",this());
      if(effect)
      {
        EffectVar(0,this(),effect) = GetX();
        EffectVar(1,this(),effect) = GetY();
      }

      var oldtrail = pTrail;
      CreateTrail(oldtrail->LocalN("w")/1000*20, oldtrail->LocalN("l")/1000*100);
      
      oldtrail->SetPosition(GetX()+iX,GetY()+iY);
      oldtrail->SetXDir();
      oldtrail->SetYDir();
      //oldtrail->RemoveObject();
    }
    
    return(true);
  }
}

private func Hit()
{
  //*hrhrhr*
  if(!Fast())
    HitLandscape();
}

private func HitLandscape()
{
  var x,y;
  if(Fast())
  {
    x = lx;
    y = ly;
  }
  OnHitLandscape(x,y);

  // umliegende Objekte beschädigen.
  var objs = FindObjects(Find_AtPoint(),
  											 Find_NoContainer(),
												 Find_Or
                         (
                           Find_Func("IsBulletTarget",GetID(),this(),shooter),
                           Find_OCF(OCF_Alive)
                         ),
                         Find_Func("CheckEnemy",this()),
                         Find_Not(Find_Func(ObjectCall(this(),"HitExclude"))));  
  {
    for(var pTarget in objs)
      HitObject(pTarget);
  }

  HitObject();//Wuff! Hier kann CoD4-Like Materialdurchschlag ansetzen.  O_O
}

public func OnHitLandscape(int iX, int iY)
{
  if(Fast())
  {
    var tmp = CreateObject(TRAI,iX,iY,-1);
    tmp->Sound("BulletHit*.ogg");
    tmp->RemoveObject();
    
    var rgb = 0;

    if(GBackSolid(iX,iY))
    {    
      var mat = GetMaterial(iX,iY);
      var rand = Random(3);
      rgb = RGB(GetMaterialColor(mat,rand,0),
                GetMaterialColor(mat,rand,1),
                GetMaterialColor(mat,rand,2));
    }
    
    if(rgb)
    {
      //Partikeleffekte
      var alphamod, sizemod;
      CalcLight(alphamod, sizemod);
      SmokeBurst((100*GetCon()/100)*sizemod/100,iX,iY,GetR()-180,0,rgb);
    }
  }
  else
  {
    Sound("BulletHit*.ogg");
  }
}


public func HitObject(object pObject)
{
  if(shooter && pObject)
    if(pObject == shooter)
      return(false);//>:O

  if(BulletStrike(pObject))
  {
    var x,y;
    if(Fast())
    {
      x = lx;
      y = ly;
    }
    OnBulletHit(pObject,x,y);
    
    Remove();
    return(true);
  }
}

public func OnBulletHit(object pObject, int iX, int iY)
{
  if(pObject)
  {
    if(Fast())
    {
      var tmp = CreateObject(TRAI,iX,iY,-1);
      tmp->Sound("BulletImpact*.ogg");
      tmp->RemoveObject();
    }
    else
    {
      Sound("BulletImpact*.ogg");
    }
    
    if(GetOCF(pObject) & OCF_Living)//Für Lebewesen keine Funken-Effekte.
      return();
  }
  
  // Partikel verschleudern
  var iAngle, iSpeed, iSize;
  
  for( var i=0; i < (30*GetCon()/100); ++i)
  {
    iAngle = (GetR()-180)+RandomX(-20,+20);
    iSpeed = RandomX(0,speed/9);
    iSize = RandomX(10,40);
    CreateParticle("Frazzle",iX+Sin(iAngle,2),iY-Cos(iAngle,2),+Sin(iAngle,iSpeed),-Cos(iAngle,iSpeed),iSize,GlowColor(iTime));
  }

  //....peng!
  var alphamod, sizemod, iColor = GlowColor(iTime);
  CalcLight(alphamod, sizemod);
  
  var r,g,b,a;
  SplitRGBaValue(iColor,r,g,b,a);
  iColor = RGBa(r,g,b,Min(a+alphamod,255));
  
  if(GetEffectData(EFSM_BulletEffects) > 0) CreateParticle("Flare",iX,iY,RandomX(-1000,+1000),RandomX(-1000,+1000),(100*GetCon()/100)*5*sizemod/100,iColor);
}

public func BulletStrike(object pObj)
{
  if(pObj)
    DoDmg(iDamage,DMG_Projectile,pObj,0,0,0,wpnid);
  return(true);
}

private func Color(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2));
}

public func TrailColor(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2));
}

public func GlowColor(int iATime)
{
  return(RGBa(255,190,0,50));
}

// Dummy-func
func HitExclude()
{

}

public func Remove(int iRemoveTime)
{
  if(pTrail)
  {
    pTrail->SetPosition(GetX(),GetY());
    pTrail->Remove();
  }
  
  if(Fast())
  {
    if(!GetEffect("Remove",this()))
    {
      BulletParticle();//Tataa. *effekt*
      //SetVisibility(VIS_None);
      SetXDir();
      SetYDir();
      SetClrModulation(RGBa(255,255,255,255));//Unsichtbar... - Lol, wie hackig.
      AddEffect("Remove",this(),1,Max(1,iRemoveTime),0,GetID());
    }
  }
  else
    RemoveObject();
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
  if(temp) return();
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  if(!byObj)
    byObj = target;
  if(byObj->Contained())
    byObj = (byObj->Contained());
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
  var dist = Distance(oldx, oldy, newx, newy);
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  
  // Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  // Ja, wir treffen nur uns selbst nicht (ja, Workaround .)
  if(EffectVar(4, target, effect)) exclude = target;
  
  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (entfernten zuerst, da wir die zuerst passieren)
  for(obj in FindObjects(Find_OnLine(oldx,oldy,newx,newy),
                         Find_NoContainer(),
                         Sort_Distance(oldx, oldy)))
  {
		// Excludes
		if(obj == target) continue;
		if(obj == exclude) continue;
		
		// CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect), oldx, oldy) || GetOCF(obj) & OCF_Alive) {
			DebugLog("%s IsBulletTarget: %i, %s, %s","HitCheck",GetName(obj),GetID(target),GetName(target),GetName(EffectVar(2, target, effect)));
			return(target-> ~HitObject(obj));
	  }
  }

  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  //verdammt, kommentier doch mal... Also:
  // Der Schuss wird erst "scharf gemacht", d.h. kann den Schützen selbst treffen, wenn
  // der Schuss einmal die Shape des Schützen verlassen hat.
  
  // OKOK, Ich hab sogar das Restzeug kommentiert. :P
  if(!EffectVar(5,target,effect)) {
	  if(!EffectVar(4, target, effect)) {
	    // ready gibt an, ob wir schon "scharf" sind. True = Scharf
	    var ready = true;
	    // Wir suchen alle Objekte mit der ID unseres Schützens an unserer momentanen Stelle
	    for(var foo in FindObjects(Find_AtPoint(GetX(target),GetY(target)),Find_ID(EffectVar(3, target, effect))))
	    	// und schauen, ob es der Schütze ist.
	      if(foo == EffectVar(2, target, effect))
	        // wir haben den Schützen gefunden -> Er ist noch an unserer Position
	        ready = false;
	    // wir haben den Schützen nicht gefunden
	    if(ready)
	      // -> Wir treffen ihn ab jetzt.
	      EffectVar(4, target, effect) = true;
	  }
  }
}

// Ist ein Schuss! Nicht warpen!
func NoWarp() { return(true); }
// Ist ein Schuss!
func IsBullet() { return(true); }


private func HitCheck(int r, int d)
{
  // calculate length
  var mx = +Sin(r,d),
      my = -Cos(r,d),
      ml = 0;
      
  while(++ml)
  {
    if(GBackSolid(mx*ml/d,my*ml/d))
      break;
    if(ml>=d)
      break;
  }
  
  mx = +Sin(r,ml);
  my = -Cos(r,ml);

  for(var i = 0; i < ml; i++ /*i=Min(ml,i++)*/)
  {
    var pObj;

    for(pObj in FindObjects(Find_AtPoint(NotZero(mx*i/ml),my*i/ml),
                         Find_Exclude(this()),
                         Find_Exclude(shooter),
                         Find_NoContainer(),
                         Find_Or(Find_Func("IsBulletTarget",GetID(),this(),shooter), Find_OCF(OCF_Alive)),
                         Find_Func("CheckEnemy",this()),
                         Find_Not(Find_Func("HitExclude"))))
    {
      lx = mx*i/ml;
      ly = my*i/ml;
      
      if(HitObject(pObj))
      {
        //lx = GetX(pObj);
        //ly = GetY(pObj);//O_o
        dst += i;
        return(i);
      }
    }
  }
  
  lx = mx;
  ly = my;
  
  if(ml < d)//Nicht in der Luft. :O
    HitLandscape(mx,my);

  dst += ml;
  return(ml);
}

private func NotZero(int a)
{
  if(!a) return(1);
  else return(a);
}

public func FxRemoveTimer(object target, int effect, int time)
{
  RemoveObject(target);
  return(-1);
}

public func GetKillIcon()
{
  return(wpnid);
}

public func BulletParticle()
{
  if(!Fast()) return();
  //if(!Random(25-Min(GetCon()/4,25))) return();

  var l = Distance(lx,ly)-20;//-hinten
  if(l < 40) return();//hinten + vorne
  //if(Random((200-Min(l,200))) < 100) return();
  
  var p = 20+Random(l-20);
  var s = 30+Random(10);

  CreateParticle("BulletTail",lx*p/l,ly*p/l,
                             +Sin(GetR(),s),
                             -Cos(GetR(),s),
                             (200*GetCon()/100)*5,GlowColor(iTime));
}

global func GetShooter(object weapon)
{
  var shooter;

  if(!weapon) return;
  
  // Waffe?
  if(weapon->~IsWeapon())
  {
    shooter = weapon->GetUser();
	// Schütze ist irgendwo drin
	if(Contained(shooter))
	  shooter = Contained(shooter);
	//oder fässt was an
	if(shooter)
	  if(shooter->GetActionTarget() && (shooter->GetProcedure() == "PUSH"))//!!!
	    shooter = GetActionTarget();
  }
  
  // noch immer nichst gefunden?
  if(!shooter)
    // dann einfach weapon nehmen.
    shooter = weapon;
	
	return shooter;
}

/* SpecialAmmo-System */

//public func IsSpecialAmmo(){return(SHTX == GetID());}
public func IsSpecialAmmo(){return(true);}//Fun mit Sharpnelz! xD

public func CustomLaunch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime)
{
  return(Launch(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime));
}

public func CustomBulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor)
{
	if(!GetEffectData(EFSM_BulletCasing)) return;
  return(BulletCasing(iX,iY,iXDir,iYDir,iSize,iColor));
}

public func CustomMuzzleFlash(int iSize, object pTarget, int iX, int iY, int iAngle, int iColor)
{
  return(MuzzleFlash(iSize,pTarget,iX,iY,iAngle,iColor));
}

public func FMMod(int iType,Data)
{
  return(Data);
}
