--���ﺰ���ű�

-- �������ս������
function c1004319_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10041)
       end	
end

aux.RegisterCreatureEvent(1004319, 2, "c1004319_OnEnterCombat")

