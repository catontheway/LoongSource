--���ﺰ���ű�

-- �������ս������
function c1005221_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20028)
       end	
end

aux.RegisterCreatureEvent(1005221, 2, "c1005221_OnEnterCombat")

