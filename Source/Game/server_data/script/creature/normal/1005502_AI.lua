--���ﺰ���ű�

-- �������ս������
function c1005502_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20096)	
end

aux.RegisterCreatureEvent(1005502, 2, "c1005502_OnEnterCombat")

