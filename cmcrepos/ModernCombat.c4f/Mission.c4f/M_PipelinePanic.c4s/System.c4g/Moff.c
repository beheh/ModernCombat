/*-- Moff --*/

//Appendto der Zukunft.
//Hier wird das Springen für die alten Moffs nachgebesert.

#strict
#appendto MF5B

func Angriff()
{
  var Clonk;
  var Queen;
  var Abstand;

  Clonk=FindObject(0,-25,-25,50,50,OCF_Prey(),0,0,NoContainer());
  Queen=FindObject(KG5B,0,0,0,0,0,"Lauern",Clonk);

  if(!Clonk)                                           return(0);
  if(Stuck())                                          return(0);
  if((GetAction()ne"Walk")&&(GetAction()ne"Swim"))     return(0);
  //if(!PathFree(GetX(),GetY(),GetX(Clonk),GetY(Clonk))) return(0);//hihihi ... *schadenfrohes Lachen*

  if(Queen) 
  {
    SetAction("Werfen",Clonk,Queen);
    Abstand=(GetX(Queen)-GetX(Clonk));
  }
  else
  {
    SetAction("Hacken",Clonk);
    Abstand=(GetX(Clonk)-GetX());
  }
  if(Abstand>0) SetDir(DIR_Right()); else SetDir(DIR_Left());
  
  if(Inside(ObjectDistance(Clonk,0),10,46))//Jetzt springt er sie auch an! *harhar*
    Jump();
  else
  {
    var x = 30;//Abstand
    if(GetDir() == DIR_Left())
      x = -x;
    if(GBackSolid (x,3))//3 runter gehen
      Jump();
  }

  return(1);
}

func Verteilen()
{
  if(FindObject(KG5B,0,0,0,0,0,"Halten",this())) return(0);

  if(FindObject(MF5B,-25,-25,50,50))
  {   
    SetCommand(this(),"None");

    if(Random(2)) SetComDir(COMD_Left());
    else          SetComDir(COMD_Right());
  }
  if(!Inside(GetX(),50,LandscapeWidth()-50))
  {
    SetCommand(this(),"None");
    
    if(GetX()<50) SetComDir(COMD_Right());
    else          SetComDir(COMD_Left());
  }

    //Auch mal hochspringen
    var x = 30;//Abstand
    if(GetDir() == DIR_Left())
      x = -x;
    if(GBackSolid (x,3))//3 runter gehen
      Jump();

  return(1);
}