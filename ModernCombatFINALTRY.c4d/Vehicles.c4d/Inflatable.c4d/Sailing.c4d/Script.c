/*-- Schlauchboot --*/

#strict 2

local motor, motoridle, turn_end_dir, damaged;

public func MaxDamage()	{return 150;}


/* Initalisierung */

protected func Initialize()
{
  SetUp();
}

func SetUp()
{
  SetAction("Sailing");
  SetComDir(COMD_None);
  var dir = GetDir();
  SetDir(DIR_Left);

  turn_end_dir = -1;
  motor = CreateObject(OBMT,0,0,GetOwner());
  motor->SetBoat(this);
  motor->UpdateDmg();
  
  SetDir(dir);
  TurnEnd();
  return 1;
}

/* Zerstörung */

private func UpdateDmg()
{
  if(!GetActionTarget()) return;
  DoDamage(GetDamage() - GetDamage(GetActionTarget()), GetActionTarget());
}

public func Damage()
{
  UpdateDmg();
  if(GetDamage() < MaxDamage()) return;
  Incineration();
}

func Incineration()
{
  if(GetID(this) != INFS) return;
  if(damaged) return;
  damaged = true;

  if(motor) {
  	motor->SetOwner(NO_OWNER);
  	motor->SetController(NO_OWNER);
  	motor->FakeExplode(20);
		RemoveObject(motor);
	}
	
  //Effekte
  CreateParticle("Blast",0,-10,-20,0,5*50,RGB(255,255,128));
  CreateParticle("Blast",0,-10,20,0,5*50,RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",15,15,0,-10,100,200,RGBa(0,0,0,100));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,0,0,20,170,RGB(50,250,50));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,100,0,0,30,100,RGB(0,0,0));
  Sound("MotorIdleLoop.ogg",false,motoridle,100,0,-1);
  Sound("MotorLoop.ogg",false,motor,100,0,-1);
  Sound("OutOfAir.ogg");
  Sound("StructuralDamage*.ogg");

  //Verschwinden
  Schedule("ChangeDef(INFB)",1);
  FadeOut();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
  return;
}

/* Landung */

private func LandOn()
{  
  Stop();
  RemoveObject(motor);
  ChangeDef(INFL);
  SetAction("JustLanded");
  Sound("MotorIdleLoop.ogg",false,motoridle,100,0,-1);
}

private func SoundSailDown()	{Sound("SailDown");}
private func SoundSailUp()	{Sound("SailUp");}

/* Bewegung */

private func Sail()
{
  if(!FindObject(0,0,0,0,0,0,"Push",motor))
    Stop();
    
  if(!GetComDir()) return ;

  var xdir = Min(Abs(GetXDir())+2,30);

  Sound("MotorLoop.ogg",false,motor,100,0,+1);
  Sound("MotorIdleLoop.ogg",false,motoridle,100,0,-1);
  
  if(!IsTurning())
    if(GetComDir() == COMD_Left)
      SetXDir(-xdir);
    else if(GetComDir() == COMD_Right)
      SetXDir(+xdir);

  //Effekte
  var dir = -(GetDir()*2-1);

  var bubble = CreateObject(FXU1,dir*30,+4,-1);
  if(bubble)
  {
    SetXDir(8, bubble);
    SetYDir(5, bubble);
  }
  CreateParticle("SlimeGrav",30*dir,-5,(1+Random(4))*dir,-(7+Random(4)) ,xdir*100/30 ,RGBa(100,150,255,100+Random(100)));
  CreateParticle("SlimeGrav",30*dir,-10,(1+Random(4))*dir,-(7+Random(4)) ,xdir*100/30 ,RGBa(100,150,255,100+Random(100)));
}

/* Steuerung */

func Right()
{
  if(GetDir() == DIR_Left)
    SetDirection(COMD_Right);
  if(GetAction() == "Turn")
    return ;
  SetComDir(COMD_Right);
}

func Left()
{
  if(GetDir() == DIR_Right)
    SetDirection(COMD_Left);
  if(GetAction() == "Turn")
    return ;
  SetComDir(COMD_Left);
}

func Stop()
{
  SetComDir(COMD_None);
  Sound("MotorLoop.ogg",false,motor,100,0,-1);

  if(FindObject(0,0,0,0,0,0,"Push",motor))
   Sound("MotorIdleLoop.ogg",false,motoridle,100,0,+1);
}

/* Kontrolle ob verwendet */

public func GetActionTarget()
{
  return motor;
}

/* Drehen */

private func SetDirection(int comdir)
{
  // Richtungsaenderung nach oben/unten geht auch mit "Turn", aber eine
  // ComDir-Aenderung, die wieder eine Turn-Action erfordern wuerde muss
  // warten, bis die jetzige Turn-Action fertig ist.
  if(GetAction() == "Turn")
  {
    turn_end_dir = comdir;
    if(comdir == COMD_Stop || (ComDirLike(comdir, COMD_Right) && GetDir() == DIR_Left) || (ComDirLike(comdir, COMD_Left) && GetDir() == DIR_Right))
    {
      return 0;
    }
  }

  // ComDir uebernehmen
  SetComDir(comdir);

  if(ComDirLike(comdir, COMD_Right) && GetDir() == DIR_Left)
  {
    SetDir(DIR_Right);
    SetAction("Turn");
  }

  if(ComDirLike(comdir, COMD_Left) && GetDir() == DIR_Right)
  {
    SetDir(DIR_Left);
    SetAction("Turn");
  }
}

private func TurnStart()
{
  SetShape(-(GetDefWidth()/2),-(GetDefHeight()/2),GetDefWidth(),GetDefHeight(), motor);

  var controllers = CreateArray();
  for(var obj in FindObjects(Find_InRect(-(GetDefWidth()/2),-GetDefHeight(),GetDefWidth(),GetDefHeight()),Find_Category(C4D_Vehicle|C4D_Object|C4D_Living)))
  {
    if (GetContact(obj, -1, 8))
    {
      controllers[GetLength(controllers)] = obj;
    }
  }
  
  //controllers = FindObjects(Find_InRect(-(GetDefWidth()/2),-GetDefHeight(),GetDefWidth(),GetDefHeight()),Find_Category(C4D_Vehicle|C4D_Object|C4D_Living));
  
  AddEffect("IntTurn", this, 1, 1, this, 0, controllers);
}

private func Turning()
{
  if(motor)
    motor->UpdateShape();
}

public func IsTurning() {
  return (GetEffect("IntTurn", this) != 0);
}

protected func FxIntTurnStart(object target, int number, int temp, array controllers)
{
  if(temp) return;

  // Alle anfassenden Clonks
  EffectVar(0, target, number) = controllers;
  // Deren initiale Position...
  EffectVar(1, target, number) = CreateArray(GetLength(controllers));
  // ... die hier ermittelt wird
  for(var i = 0; i < GetLength(controllers); ++ i)
    EffectVar(1, target, number)[i] = GetX(controllers[i]) - GetX();
  //Anfangsgeschwindigkeit
  EffectVar(2, target, number) = Abs(GetXDir(target));
}

protected func FxIntTurnTimer(object target, int number, int time)
{
  // Laenge der Turn-Action bestimmen
  var delay = GetActMapVal("Delay", "Turn");
  var phases = GetActMapVal("Length", "Turn");
  // Schon vorbei
  if(time == delay * phases) return -1;
  // Eine Sinus-Kurve von -90 bis +90 Grad hernehmen um die neue Position
  // der Clonks zu bestimmen.
  var sin_phase = (time * 180 / (phases * delay)) - 90;
  var pos_phase = Sin(sin_phase, 50) + 50;
  // Clonkpositionen anpassen
  var controller_count = GetLength(EffectVar(0, target, number));
  for(var i = 0; i < controller_count; ++ i)
  {
    var controller = EffectVar(0, target, number)[i];
    var pos = EffectVar(1, target, number)[i];

    // Koennte sein, dass ein Controller verloren geht
    if(!controller) continue;

    // Dir anpassen wenn Haelfte der Turn-Action vorbei
    if(time == delay * phases / 2)
      SetDir(GetDir(), controller);

    // Neue Position ermitteln (Sinusfoermig um den Luftschiff-Mittelpunkt)
    var pos_t = -pos * 2;
    SetPosition(GetX() + pos - (pos*2 * pos_phase / 100), GetY(controller), controller);
  }
  var speed_phase = (time * 180 / (phases * delay)) - 90;
  var xdir = speed_phase * EffectVar(2, target, number) / 90;
  if(GetComDir(target) == COMD_Left)
    SetXDir(-xdir, target);
  else if(GetComDir(target) == COMD_Right)
    SetXDir(+xdir, target);
}

protected func FxIntTurnStop(object target, int number, bool temp)
{
  if(temp) return 0;
  var length = GetLength(EffectVar(0, target, number));

  // Nochmal alle Controller durchgehen
  for(var i = 0; i < length; ++ i)
  {
    var object = EffectVar(0, target, number)[i];
    var pos = EffectVar(1, target, number)[i];

    // Und an fertige Position tun, zur Sicherheit, wenn es noch Controller sind
    if(!object) continue;
    if(GetAction(object) == "Push" && GetActionTarget(0, object) == this)
      SetPosition(GetX() - pos, GetY(object), object);
  }
}

private func TurnEnd()
{
  if(turn_end_dir != -1)
  {
    SetDirection(turn_end_dir);
    turn_end_dir = -1;
  }
  
  motor->UpdateShape();
}
