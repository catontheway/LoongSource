--���ﺰ���ű�

-- �������ս������
function c1004130_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10017)
       end	
end

aux.RegisterCreatureEvent(1004130, 2, "c1004130_OnEnterCombat")



