--���ﺰ���ű�

-- �������ս������
function c1003024_OnEnterCombat(MapID, InstanceID, CreatureID)

            cre.MonsterSay(MapID, InstanceID, CreatureID, 3)
end

aux.RegisterCreatureEvent(1003024, 2, "c1003024_OnEnterCombat")

