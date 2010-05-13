/*-- Zerstörte Stahlbrücke --*/

#strict 2


func Hit3()
{
   //Effekte
   CastParticles("ConcreteSplinter",4,150,0,0,40,100);
   CastParticles("ConcreteSplinter",8,230,0,0,40,15,RGB(40,20,20));
   CastParticles("Smoke3",25,15,0,0,50,200);
   Sound("StructuralDamage*.ogg");
}