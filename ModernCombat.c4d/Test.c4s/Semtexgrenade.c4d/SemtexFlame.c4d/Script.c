/*-- Anzünderobjekt --*/
 
#strict 2

local slimy, iDamage;
local shooter; // Dingens/Clonk das den Schuss abgefeuert hat.
local hits;
local maxlifetime;
local size;

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return;
  // Waffe?
  shooter = GetShooter(byObj);
  
  maxlifetime = 40;
  hits = 0;
}


public func MaxLifeTime() { return maxlifetime; }

public func SetMaxLifetime(int max) {
	maxlifetime = max;
}

public func StdSize(){return iDamage*2;}

public func Launch(int xdir, int ydir, bool slime, int iDamagepar)
{
  slimy = slime;
  iDamage = iDamagepar;
  if(!iDamage)
    iDamage = 1;

  SetSpeed(xdir,ydir);
  SetAction("Fly");
  size = StdSize();
  AddEffect("HitCheck", this(), 1,1, this(),0,shooter);
}


protected func Flying()
{
  if(InLiquid()) {
    // Blubberblubber
    for(var i=0; i<5; ++i) Bubble(RandomX(-5,+5),RandomX(-5,5));
    RemoveObject();
	  return;
  }

  if(GetActTime() > MaxLifeTime()-Random(5)) {
    RemoveObject();
    return;
  }
  
  if(!Random(4) && GetActTime() > MaxLifeTime()/4)
    SetYDir(GetYDir()+RandomX(-4,2));

  if(slimy) {
    CastSlimeEffects(GetActTime());
  } else {
    CastFireEffects(GetActTime());
  }
  
  size+=3;
  /*if(size >= StdSize()*2)
  {
    var flame = CreateObject(SFLM,0,0,GetController());
    iDamage /= 2;
    flame->SetMaxLifetime(maxlifetime);
    var x,y,a = Angle(GetXDir(),GetYDir()),s = Distance(GetXDir(),GetYDir());
    
    var a1 = a+RandomX(-10,+10);
    x = +Cos(a1,s);
    y = -Sin(a1,s);
    flame->Launch(x,y,0,iDamage);
 
    size = StdSize();
    var a1 = a+RandomX(-10,+10);
    x = +Cos(a1,s);
    y = -Sin(a1,s);
    SetSpeed(x,y);
  }*/
  
  SetYDir(GetYDir(0,100)+GetGravityAccel4K(100)/7,0,100);
}

public func HitObject(object pObj)
{
  var color = FIRE_Green;
  // 100..70
  var schwaecher = Max(100 - hits*20,0);
  //Message("@%d%",this,schwaecher);
  // Energie abziehen und langsamer machen
  if(!slimy) {
    color = FIRE_Red;
    DoDmg(iDamage*schwaecher/100, DMG_Fire, pObj,1);
  }
  else {
    DoDmg(iDamage*schwaecher/100, DMG_Bio, pObj, 1);
  }

  hits++;
  
  //anzündbares anzünden, aber nicht lebewesen.
  if(pObj) //existiert es überhaupt noch?
    if(pObj->GetOCF() & OCF_Inflammable) {
      if(!(pObj->GetOCF() & OCF_Living))
        Incinerate(pObj);
	  else
	    AddFireEffect(pObj,50,color,1);
	}
}

protected func CastFireEffects(int time)
{
  var maxtime = MaxLifeTime();
  var dir = (GetXDir()>0)*2-1;
  var angle = Angle(0,0,GetXDir(),GetYDir());
  
  var ntime = 40*time/maxtime;

  for (var i = 0; i < 3; i++) 
  {

    var xoff = RandomX(-2,5)+ntime/2;
	var yoff = RandomX(-ntime , ntime);
  
    var xdir = +Cos(angle,yoff)-Sin(angle,xoff);
    var ydir = +Sin(angle,yoff)+Cos(angle,xoff);

	var x = (+10-2*i)*dir;
    var y = RandomX(-size,size)/20;

	var alpha = BoundBy(180+(time-maxtime*3/4)*2,0,255);
    var color = RGBa(255, 255, 255, alpha);

    CreateParticle("Fire2", x, y, xdir, ydir, size, color);
  }  
  
}

protected func CastSlimeEffects(int time)
{
  var maxtime = MaxLifeTime();
  var dir = (GetXDir()>0)*2-1;
  var angle = Angle(0,0,GetXDir(),GetYDir());
  
  var ntime = 40*time/maxtime;
  
  for (var i = 0; i < 3; i++) 
  {

    var xoff = RandomX(-2,5);
	var yoff = RandomX(-ntime , ntime)/2;
  
    var xdir = +Cos(angle,yoff)-Sin(angle,xoff);
    var ydir = +Sin(angle,yoff)+Cos(angle,xoff);

	var x = (+10-2*i)*dir;
    var y = RandomX(-size,size)/20;

	var alpha = BoundBy(ntime ,0,255);
    var color = RGBa(100, 255, 0, alpha);

    CreateParticle("AlienPlasma", x, y, xdir, ydir, size, color);
  }  
}

/* Effekt für Trefferüberprüfung */

// weitgehend kopiert aus Schuss :(

// EffectVars:
// 2 - Schütze (Objekt, das die Waffe abgefeuert hat, üblicherweise ein Clonk)
// 3 - ID des Schützen
// 4 - Scharf? Wenn true wird der Schütze vom Projektil getroffen 

private func FxHitCheckStart(object target, int effect, int temp, object byObj)
{
  if(temp) return;
  if(!byObj)
    byObj = target;
  if(byObj->Contained())
    byObj = (byObj->Contained());
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = false;
}

private func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  // Oh man. :O
  var x = GetX(target);
  var y = GetY(target);
  var angle = Angle(0,0,GetXDir(target),GetYDir(target));
  var maxlife = target->~MaxLifeTime();
  var wdt = 20 + Abs(Cos(angle, size));
  var hgt = 20 + Abs(Sin(angle, size));
  
  //target->SetObjDrawTransform(wdt*1000/4,0,0,0,hgt*1000/4);
  
  // Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  if(EffectVar(4, target, effect)) exclude = target;
  
  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (nähere zuerst)

  for(obj in FindObjects(Find_InRect(x-wdt/2,y-hgt/2,wdt,hgt),Find_NoContainer()))
  {
		// Excludes
		if(obj == target) continue;
		if(obj == exclude) continue;
		
		// CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect)) || GetOCF(obj) & OCF_Alive) {
			return target->~HitObject(obj);
	  }
  }
  
  // OKOK, Ich hab sogar das Restzeug kommentiert. :P
  if(!EffectVar(5,target,effect)) {
	  if(!EffectVar(4, target, effect)) {
	    // ready gibt an, ob wir schon "scharf" sind. True = Scharf
	    var ready = true;
	    // Wir suchen alle Objekte mit der ID unseres Schützens an unserer momentanen Stelle
	    for(var foo in FindObjects(Find_AtPoint(x,y),Find_ID(EffectVar(3, target, effect))))
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

protected func Hit(int iXDir, int iYDir)
{
  if(!iXDir) iXDir = GetXDir(0,100);
  if(!iYDir) iYDir = GetYDir(0,100);
  
  var d = Distance(iXDir,iYDir)/4;
  iXDir += RandomX(-d,+d);
  iYDir += RandomX(-d,+d);

  if(GBackSolid(0,+5)) return(SetYDir(-iYDir,0,100));
  if(GBackSolid(0,-5))  return(SetYDir(-iYDir,0,100));
  if(GBackSolid(-5,0))  return(SetXDir(-iXDir,0,100));
  if(GBackSolid(+5,0))   return(SetXDir(-iXDir,0,100));
}

func NoWarp() { return true; }
