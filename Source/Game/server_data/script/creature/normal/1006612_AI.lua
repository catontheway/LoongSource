--���ﺰ���ű�

-- �������ս������
function c1006612_OnEnterCombat(MapID, InstanceID, CreatureID)
             
         cre.MonsterSay(MapID, InstanceID, CreatureID, 30109)
end

aux.RegisterCreatureEvent(1006612, 2, "c1006612_OnEnterCombat")

--������������
function c1006612_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
             cre.MonsterSay(MapID, InstanceID, TargetID, 30111)	

end

aux.RegisterCreatureEvent(1006612, 4, "c1006612_OnDie")


