--���ﺰ���ű�

-- �������ս������
function c1010104_OnEnterCombat(MapID, InstanceID, CreatureID)
        local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
	      cre.MonsterSay(MapID, InstanceID, CreatureID, 50050)
	end
end

aux.RegisterCreatureEvent(1010104, 2, "c1010104_OnEnterCombat")


