--���ﺰ���ű�

-- �������ս������
function c1003065_OnEnterCombat(MapID, InstanceID, CreatureID)
       
             cre.MonsterSay(MapID, InstanceID, CreatureID, 1)	
end

aux.RegisterCreatureEvent(1003065, 2, "c1003065_OnEnterCombat")

