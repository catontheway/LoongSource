--���ﺰ���ű�

-- �������ս������
function c1007029_OnEnterCombat(MapID, InstanceID, CreatureID)
           
        cre.MonsterSay(MapID, InstanceID, CreatureID, 40008)
       
end

aux.RegisterCreatureEvent(1007029, 2, "c1007029_OnEnterCombat")


--������������
function c1007029_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	          
   cre.MonsterSay(MapID, InstanceID, TargetID, 40009)
       

end

aux.RegisterCreatureEvent(1007029, 4, "c1007029_OnDie")

