--���ﺰ���ű�

-- �������ս������
function c1009305_OnEnterCombat(MapID, InstanceID, CreatureID)
        local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
	      cre.MonsterSay(MapID, InstanceID, CreatureID, 50030)
	end
end

aux.RegisterCreatureEvent(1009305, 2, "c1009305_OnEnterCombat")


