/*-- Schlauchboot (aktiv) --*/

#strict

local motor,turn_end_dir;

func SetUp()
{
  SetAction("Sailing");
  SetComDir(COMD_None);
  var dir = GetDir();
  SetDir(DIR_Left);

  turn_end_dir = -1;
  motor = CreateObject(OBMT,0,0,GetOwner());
  motor->SetBoat(this());
  motor->UpdateDmg();
  
  SetDir(dir);
  TurnEnd();
  return(1);
}

public func MaxDamage()		{ return(50); }

private func UpdateDmg()
{
  if(!GetActionTarget()) return;
  GetActionTarget()->DoDamage(GetDamage() - GetActionTarget()->GetDamage());
}

public func Damage()
{
  UpdateDmg();
	if(GetDamage() < MaxDamage()) return();
	Incineration();
}

func Incineration()
{
 Sound("OutOfAir");
 if(motor) Explode(20,motor);
 CastParticles("XSpark", 10, 40, RandomX(-38,38), RandomX(-2,3), 50, 0, RGB(190,85,20));
 FadeOut();
 ChangeDef(INFB);
}

private func LandOn()
{  
  Stop();
  RemoveObject(motor);
  ChangeDef(INFL);
  SetAction("JustLanded");
}

private func SoundSailDown() { Sound("SailDown"); }
private func SoundSailUp()   { Sound("SailUp");   }

/* Der Antrieb */

private func Sail()
{
  if(!FindObject(0,0,0,0,0,0,"Push",motor))
    Stop();
    
  if(GetComDir() == COMD_None) return();

  var xdir = Min(Abs(GetXDir())+2,30);
  
  Sound("motor_loop",false,motor,100,0,+1);
 
  if(GetComDir() == COMD_Left)
    SetXDir(-xdir);
  else
    SetXDir(+xdir);
    

  //Effekte
  var dir = -(GetDir()*2-1);

  var bubble = CreateObject(FXU1,dir*30,+4,-1);
  if(bubble)
  {
    bubble->SetXDir(8);
    bubble->SetYDir(5);
  }

  CreateParticle("SlimeGrav",30*dir,-5,(1+Random(4))*dir,-(7+Random(4)) ,xdir*100/30 ,RGBa(100,150,255,100+Random(100)));
  CreateParticle("SlimeGrav",30*dir,-10,(1+Random(4))*dir,-(7+Random(4)) ,xdir*100/30 ,RGBa(100,150,255,100+Random(100)));
}


/* Stuerung */
func Right()
{
  if(GetDir() == DIR_Left)
    SetDirection(COMD_Right);
  if(GetAction() eq "Turn")
    return();
  SetComDir(COMD_Right);
}

func Left()
{
  if(GetDir() == DIR_Right)
    SetDirection(COMD_Left);
  if(GetAction() eq "Turn")
    return();
  SetComDir(COMD_Left);
}

func Stop()
{
  SetComDir(COMD_None);
  Sound("motor_loop",false,motor,100,0,-1);
}


/* Drehen */

private func SetDirection(int comdir)
{
  // Richtungsaenderung nach oben/unten geht auch mit "Turn", aber eine
  // ComDir-Aenderung, die wieder eine Turn-Action erfordern wuerde muss
  // warten, bis die jetzige Turn-Action fertig ist.
  if(GetAction() S= "Turn")
  {
    turn_end_dir = comdir;
    if(comdir == COMD_Stop || (ComDirLike(comdir, COMD_Right) && GetDir() == DIR_Left) || (ComDirLike(comdir, COMD_Left) && GetDir() == DIR_Right))
    {
      return(0);
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


//Vom Luftschiff kopiert.
private func TurnStart()
{
  motor->SetShape(-(GetDefWidth()/2),-(GetDefHeight()/2),GetDefWidth(),GetDefHeight());

  var controllers = CreateArray();
  for(var obj in FindObjects(Find_InRect(-(GetDefWidth()/2),-GetDefHeight(),GetDefWidth(),GetDefHeight()),Find_Category(C4D_Vehicle|C4D_Object|C4D_Living)))
  {
    if (obj->GetContact(0, -1, 8))
    {
      controllers[GetLength(controllers)] = obj;
    }
  }
  
  //controllers = FindObjects(Find_InRect(-(GetDefWidth()/2),-GetDefHeight(),GetDefWidth(),GetDefHeight()),Find_Category(C4D_Vehicle|C4D_Object|C4D_Living));
  
  AddEffect("IntTurn", this(), 1, 1, this(), 0, controllers);
}

private func Turning()
{
  if(motor)
    motor->UpdateShape();
}

protected func FxIntTurnStart(object target, int number, int temp, array controllers)
{
  if(temp) return(0);

  // Alle anfassenden Clonks
  EffectVar(0, target, number) = controllers;
  // Deren initiale Position...
  EffectVar(1, target, number) = CreateArray(GetLength(controllers));
  // ... die hier ermittelt wird
  for(var i = 0; i < GetLength(controllers); ++ i)
    EffectVar(1, target, number)[i] = controllers[i]->GetX() - GetX();
}

protected func FxIntTurnTimer(object target, int number, int time)
{
  // Laenge der Turn-Action bestimmen
  var delay = GetActMapVal("Delay", "Turn");
  var phases = GetActMapVal("Length", "Turn");
  // Schon vorbei
  if(time == delay * phases) return(-1);
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
      controller->SetDir(GetDir());

    // Neue Position ermitteln (Sinusfoermig um den Luftschiff-Mittelpunkt)
    var pos_t = -pos * 2;
    controller->SetPosition(GetX() + pos - (pos*2 * pos_phase / 100), controller->GetY());
  }
}

protected func FxIntTurnStop(object target, int number, bool temp)
{
  if(temp) return(0);
  var length = GetLength(EffectVar(0, target, number));

  // Nochmal alle Controller durchgehen
  for(var i = 0; i < length; ++ i)
  {
    var object = EffectVar(0, target, number)[i];
    var pos = EffectVar(1, target, number)[i];

    // Und an fertige Position tun, zur Sicherheit, wenn es noch Controller sind
    if(!object) continue;
    if(object->GetAction() S= "Push" && object->GetActionTarget() == this())
      object->SetPosition(GetX() - pos, object->GetY());
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