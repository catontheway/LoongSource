--���ﺰ���ű�

-- �������ս������
function c1010525_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 50068)
       end	
end

aux.RegisterCreatureEvent(1010525, 2, "c1010525_OnEnterCombat")

