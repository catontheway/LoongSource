--���ﺰ���ű�

-- �������ս������
function c1009009_OnEnterCombat(MapID, InstanceID, CreatureID)
        local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
	      cre.MonsterSay(MapID, InstanceID, CreatureID, 50002)
	end
end

aux.RegisterCreatureEvent(1009009, 2, "c1009009_OnEnterCombat")




