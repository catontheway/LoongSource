--���ﺰ���ű�

-- �������ս������
function c1004522_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10059)
       end	
end

aux.RegisterCreatureEvent(1004522, 2, "c1004522_OnEnterCombat")

