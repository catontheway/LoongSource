--���ﺰ���ű�

-- �������ս������
function c1290113_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 40096)
end

aux.RegisterCreatureEvent(1290113, 2, "c1290113_OnEnterCombat")



