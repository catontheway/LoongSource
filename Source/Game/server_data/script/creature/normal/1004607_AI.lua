--���ﺰ���ű�

-- �������ս������
function c1004607_OnEnterCombat(MapID, InstanceID, CreatureID)
       
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10057)

end

aux.RegisterCreatureEvent(1004607, 2, "c1004607_OnEnterCombat")

