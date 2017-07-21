/*-- Geschoss --*/

#strict 2

local fSounded;
local fHit;
local fAchievement;

public func IsBulletTarget()	{return true;}
public func AllowHitboxCheck()	{return true;}
public func IsArtilleryShell()	{return true;}

public func IgnoreEnemyCheck()	{return true;}	//Ignoriert Feindcheck bei Fahrzeugen


/* Initialisierung */

func Initialize()
{
   fSounded = false;
   fHit = false;
   fAchievement = true;
   return 1;
}

/* Rotation */

func ResetRotation()
{
  SetR(Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir()),this);

  if(GetYDir() > 2 && !fSounded)
  {
    fSounded = true;
    Sound("Artillery*.ogg");
  }

  SearchObjects();
}

/* Objekte treffen */

func SearchObjects()
{
  if(fHit) return;
  var target = FindObject2(Find_AtPoint(), Find_Exclude(this), Find_Func("IsBulletTarget", GetID(), this, 0, GetX(), GetY()));
  if(target)
  {
    if(GetOCF(target) & OCF_Alive && Hostile(GetOwner(target), GetController()))
    {
      //Achievement-Fortschritt (In the Face)
      if(fAchievement) DoAchievementProgress(1, AC16, GetController());
    }
    Hit();
  }
}

/* Schaden */

func Hit()
{
  if(fHit) return;

  //Schon getroffen
  fHit = true;

  //Explosion
  DamageObjects(50,30,this);
  Explode(30+Random(10));

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",15,10,0,0,300,700, RGBa(255,255,255,0));
  Sound("C4EX_Detonation*.ogg");
}

public func Damage()
{
  if(GetDamage() > 20)
    Hit();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}