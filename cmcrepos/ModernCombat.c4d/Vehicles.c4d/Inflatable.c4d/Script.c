/*-- Schlauchboot --*/

#strict

local motor,speed,turn_end_dir;

func MaxSpeed(){return(30);}
func Acceleration(){return(5);}
func SpeedStep(){return(5);}

func Initialize()
{
  SetAction("Be",this());
  SetComDir(COMD_Right);
  //SetDir(DIR_Right);
  turn_end_dir = -1;
  motor = CreateObject(OBMT,0,0,NO_OWNER);
  motor->SetAction("Be",this());
  motor->SetBoat(this());
  motor->UpdateShape();
  ShowSpeed();
  return(1);
}

func Incineration() {
 // Babboom :>
 /*Sound("OutOfAir");
 Explode(20, motor);
 Schedule("Smoke(RandomX(-38,38),0,10);",9,40);
 ScheduleCall(this(), "CastParticles(%s, 10, 40, RandomX(-38,38), RandomX(-2,3), 50, 0, RGB(190,85,20));", 10, 7, "XSpark");
 ChangeDef(INFB);*/
 Message("", this());
 }

/* Der Antrieb */
func Timer()
{
  if(speed)
  {
    if(speed > 0)//nach Rechts
    {
      if(GetXDir() < speed)
      {
        SetXDir(GetXDir()+Acceleration());
      }
    }
    else//nach Links
    {
      if(GetXDir() > speed)
      {
        SetXDir(GetXDir()-Acceleration());
      }
    }
  }
}


/* Stuerung */
func Right()
{
  DoSpeed(SpeedStep());
}

func Left()
{
  DoSpeed(-SpeedStep());
}

func Stop()
{
  speed = 0;
  DoSpeed(0);
}

func DoSpeed(int iSpeed)
{
  var last_dir = BoundBy(speed, -1, +1);
  speed = BoundBy(speed + iSpeed, -MaxSpeed(), MaxSpeed());
  
  ShowSpeed();
  
  if(!speed)
    Sound("motor_loop",false,motor,0,0,-1);
  else
    Sound("motor_loop",false,motor,100,0,+1);
  
  if(speed < 0)
    if(GetDir() == DIR_Left)
      return(TurnRight());
      
  if(speed > 0)
    if(GetDir() == DIR_Right)
      return(TurnLeft());
}

func ShowSpeed()
{
  var text;
  if(!speed)
    text = "O";
  else
  {
    text = "";
    for(var i = Abs(speed)/SpeedStep();i > 0;i--)
    {
      text = Format("%s-",text);
    }
    
    if(speed > 0)
    {
      text = Format("%s>",text);
    }
    else
    {
      text = Format("<%s",text);
    }
  }
  
  Message("@%s",this(),text);
}

/* Blah */
func TurnLeft()
{
  SetDirection(COMD_Left);
}

func TurnRight()
{
  SetDirection(COMD_Right);
}

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

/* Drehen */
//Vom Luftschiff kopiert.
private func TurnStart()
{
  motor->SetShape(-(GetDefWidth()/2),-(GetDefHeight()/2),GetDefWidth(),GetDefHeight());

  var controllers = CreateArray();
  for(var obj in FindObjects(Find_InRect(-(GetDefWidth()/2),-GetDefHeight(),GetDefWidth(),GetDefHeight()),Find_Category(C4D_Vehicle|C4D_Object|C4D_Living)))
  {
    if (obj->GetContact(0, -1, 8))
    {
      Message(".",obj);
      controllers[GetLength(controllers)] = obj;
    }
  }
  
  //controllers = FindObjects(Find_InRect(-(GetDefWidth()/2),-GetDefHeight(),GetDefWidth(),GetDefHeight()),Find_Category(C4D_Vehicle|C4D_Object|C4D_Living));
  
  AddEffect("IntTurn", this(), 1, 1, this(), 0, controllers);
}

protected func FxIntTurnStart(object target, int number, int temp, array controllers)
{
  if(temp) return(0);

  // Alle luftschiffanfassenden Clonks
  EffectVar(0, target, number) = controllers;
  // Deren initiale Position auf dem Luftschiff...
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
  // der Clonks auf dem Luftschiff zu bestimmen.
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
