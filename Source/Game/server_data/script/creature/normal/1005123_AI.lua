--���ﺰ���ű�

-- �������ս������
function c1005123_OnEnterCombat(MapID, InstanceID, CreatureID)
       
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20010)
	
end

aux.RegisterCreatureEvent(1005123, 2, "c1005123_OnEnterCombat")

