--���ﺰ���ű�

-- �������ս������
function c1010410_OnEnterCombat(MapID, InstanceID, CreatureID)

          cre.MonsterSay(MapID, InstanceID, CreatureID, 50063)
	
end

aux.RegisterCreatureEvent(1010410, 2, "c1010410_OnEnterCombat")



