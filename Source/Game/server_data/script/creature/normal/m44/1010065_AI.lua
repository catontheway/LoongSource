--���ﺰ���ű�

-- �������ս������
function c1010065_OnEnterCombat(MapID, InstanceID, CreatureID)
        local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
	      cre.MonsterSay(MapID, InstanceID, CreatureID, 50046)
	end
end

aux.RegisterCreatureEvent(1010065, 2, "c1010065_OnEnterCombat")



