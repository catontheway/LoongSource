--���ﺰ���ű�

-- �������ս������
function c1003127_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 21)
       end	
end

aux.RegisterCreatureEvent(1003127, 2, "c1003127_OnEnterCombat")

